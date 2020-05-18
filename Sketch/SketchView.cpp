﻿
// SketchView.cpp: CSketchView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Sketch.h"
#endif

#include "SketchDoc.h"
#include "SketchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSketchView

IMPLEMENT_DYNCREATE(CSketchView, CView)

BEGIN_MESSAGE_MAP(CSketchView, CView)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CSketchView 생성/소멸

CSketchView::CSketchView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSketchView::~CSketchView()
{
}

BOOL CSketchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSketchView 그리기

void CSketchView::OnDraw(CDC* pDC)
{
	CSketchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CPen circlePen(PS_SOLID, 2, RGB(0, 0, 0));
	HGDIOBJ circleOldPen = pDC->SelectObject(&circlePen);
	pDC->Ellipse(m_circlePt.x - m_radian, m_circlePt.y - m_radian,
		m_circlePt.x + m_radian, m_circlePt.y + m_radian);

	pDC->SelectObject(circleOldPen);
	DeleteObject(circlePen);


	if (m_clicked == TRUE)
	{
		CPen boldPen(PS_SOLID, 5, RGB(0, 0, 0));
		HGDIOBJ oldPen = pDC->SelectObject(&boldPen);
		pDC->MoveTo(m_startPt);
		pDC->LineTo(m_endPt);
		pDC->SelectObject(oldPen);
		DeleteObject(boldPen);
	}

	//CString str;
	//str.Format(_T("seta: %lf\n%d, %d"), m_seta, m_step.cx, m_step.cy);
	//pDC->DrawText(str, CRect(0, 0, 100, 100), DT_CENTER);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CSketchView 진단

#ifdef _DEBUG
void CSketchView::AssertValid() const
{
	CView::AssertValid();
}

void CSketchView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSketchDoc* CSketchView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSketchDoc)));
	return (CSketchDoc*)m_pDocument;
}
#endif //_DEBUG


// CSketchView 메시지 처리기


void CSketchView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar == VK_SPACE)
	{
		
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSketchView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_clicked = true;
	m_startPt = m_circlePt; // client pos

	CView::OnLButtonDown(nFlags, point);
}


void CSketchView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_clicked = false;

	int height = m_startPt.y - m_endPt.y;
	int width = m_startPt.x - m_endPt.x;
	m_seta = (atan2(height, width) * 180) / 3.141592;
	int r = (int)sqrt((double)(width * width + height * height));

	m_step.cx = r / 10;
	m_step.cy = r / 10;
	
	if (m_seta >= 0 && m_seta <= 90.0)
	{
		m_step.cx = abs(m_step.cx);
		m_step.cy = abs(m_step.cy);
	}
	else if (m_seta > 90.0 && m_seta <= 180.0)
	{
		m_step.cx *= -1;
		m_step.cy = abs(m_step.cy);
	}
	else if (m_seta < -90.0 && m_seta >= -180.0)
	{
		m_step.cx *= -1;
		m_step.cy *= -1;
	}
	else if (m_seta < 0 && m_seta >= -90.0)
	{
		m_step.cx = abs(m_step.cx);
		m_step.cy *= -1;
	}
	if (abs(r) / 10 >= 3)
	{
		SetTimer(1, 30, 0);
	}
	m_endPt = point;
	CView::OnLButtonUp(nFlags, point);
}


void CSketchView::OnMove(int x, int y)
{
	CView::OnMove(x, y);
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CSketchView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1 && m_clicked == false)
	{
		m_circlePt.x += m_step.cx;
		m_circlePt.y += m_step.cy;

		if (m_circlePt.y - m_radian < 1 || m_circlePt.y + m_radian > m_viewRect.Height()-3)
		{
			m_step.cy *= -1;
		}
		if (m_circlePt.x - m_radian < 1 || m_circlePt.x + m_radian > m_viewRect.Width()-3)
		{
			m_step.cx *= -1;
		}
		Invalidate();
	}
	
	CView::OnTimer(nIDEvent);
}


void CSketchView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	GetWindowRect(&m_viewRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CSketchView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_clicked == TRUE)
	{
		int height = m_startPt.y - m_endPt.y;
		int width = m_startPt.x - m_endPt.x;
		m_seta = (atan2(height, width) * 180) / 3.141592;

		Invalidate();
		m_endPt = point;
	}
	CView::OnMouseMove(nFlags, point);
}
