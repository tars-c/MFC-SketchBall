
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
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
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
	pDC->Ellipse(m_circlePt.x - m_radius, m_circlePt.y - m_radius,
		m_circlePt.x + m_radius, m_circlePt.y + m_radius);

	pDC->SelectObject(circleOldPen);
	DeleteObject(circlePen);


	if (m_bClicked == TRUE)
	{
		if (m_bPressShift == TRUE)
		{
			CPen boldPen(PS_SOLID, 5, RGB(0, 0, 0));
			HGDIOBJ oldPen = pDC->SelectObject(&boldPen);
			pDC->MoveTo(m_startPt);
			pDC->LineTo(m_fixedPt);
			pDC->SelectObject(oldPen);
			DeleteObject(boldPen);
		}
		else
		{
			CPen boldPen(PS_SOLID, 5, RGB(0, 0, 0));
			HGDIOBJ oldPen = pDC->SelectObject(&boldPen);
			pDC->MoveTo(m_startPt);
			pDC->LineTo(m_endPt);
			pDC->SelectObject(oldPen);
			DeleteObject(boldPen);
		}
	}

	CString str;
	str.Format(_T("seta: %lf\n%d, %d\nKey: %08x\n%lf"), m_seta, m_step.cx, m_step.cy, m_test, m_fixedSeta);
	pDC->DrawText(str, CRect(20, 0, 100, 100), DT_CENTER);

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

void CSketchView::OnLButtonDown(UINT nFlags, CPoint point)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(1);
	m_bClicked = TRUE;
	m_startPt = m_circlePt; // client pos
	m_endPt = point;


	Invalidate();
	CView::OnLButtonDown(nFlags, point);
}


void CSketchView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bClicked = FALSE;
	m_endPt = point;

	int height = m_startPt.y - m_endPt.y;
	int width = m_startPt.x - m_endPt.x;
	m_seta = (atan2(height, width) * 180) / 3.141592;

	int r = (int)sqrt((double)(width * width + height * height));

	m_radian = m_seta * 3.141592 / 180.0;
	r /= 10;
	m_step.cx = (int)(r * cos(m_radian));
	m_step.cy = (int)(r * sin(m_radian));



	if (abs(r) >= 3)
	{
		SetTimer(1, 50, 0);
	}
	CView::OnLButtonUp(nFlags, point);
}



void CSketchView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1 && m_bClicked == FALSE)
	{
		m_circlePt.x += m_step.cx;
		m_circlePt.y += m_step.cy;

		if (m_circlePt.y - m_radius < 1 || m_circlePt.y + m_radius >= m_viewRect.Height() )
		{
			m_step.cy *= -1;
		}
		if (m_circlePt.x - m_radius < 1 || m_circlePt.x + m_radius >= m_viewRect.Width() )
		{
			m_step.cx *= -1;
		}

		if (m_circlePt.x + m_radius >= m_viewRect.Width())
		{
			m_circlePt.x = m_viewRect.Width() - m_radius;
		}
		if (m_circlePt.y + m_radius >= m_viewRect.Height())
		{
			m_circlePt.y = m_viewRect.Height() - m_radius;
		}

		if (m_circlePt.x - m_radius < 1)
		{
			m_circlePt.x = 0 + m_radius;
		}
		if (m_circlePt.y - m_radius < 1)
		{
			m_circlePt.y = 0 + m_radius;
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
	if (m_bClicked == TRUE)
	{
		int height = m_startPt.y - m_endPt.y;
		int width = m_startPt.x - m_endPt.x;
		m_seta = (atan2(height, width) * 180) / 3.141592;

		m_endPt = point;
		int r = (int)sqrt((double)(width * width + height * height));
		Invalidate();

		if (m_bPressShift == TRUE)
		{
			const static double m_setaList[] = { -135.0, -90.0, -45.0, 0.0, 45.0, 90.0, 135.0, 180.0 };
			//const static double m_setaList[] = { -90.0, 0.0, 90.0, 180.0 };
			double diff = 0.0;
			double prevDiff = 0.0;
			for (int i = 0; i < sizeof(m_setaList) / sizeof(m_setaList[0]); ++i)
			{
				diff = abs(m_seta - m_setaList[i]);
				if (abs(diff) >= 180.0)
				{
					diff = 360.0 - diff;
				}

				if (diff > prevDiff)
				{
					prevDiff = diff;
					m_fixedSeta = m_setaList[i];
				}
			}
			m_seta = m_fixedSeta;
			CString str;
			double radian = m_fixedSeta * 3.141592 / 180.0;

			int r1 = (int)(r * cos(radian));
			int r2 = (int)(r * sin(radian));
			m_fixedPt.x = (m_circlePt.x + r1);
			m_fixedPt.y = (m_circlePt.y + r2);

			Invalidate();
		}
	}
	CView::OnMouseMove(nFlags, point);
}


void CSketchView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar == VK_SHIFT)
	{
		m_bPressShift = FALSE;
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CSketchView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CView::PreTranslateMessage(pMsg);
}



void CSketchView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CSketchView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
	{
		m_bPressShift = TRUE;

		const static double m_setaList[] = { -135.0, -90.0, -45.0, 0.0, 45.0, 90.0, 135.0, 180.0 };
		//const static double m_setaList[] = { -90.0, 0.0, 90.0, 180.0 };
		double diff = 0.0;
		double prevDiff = 0.0;
		for (int i = 0; i < sizeof(m_setaList) / sizeof(m_setaList[0]); ++i)
		{
			diff = abs(m_seta - m_setaList[i]);
			if (abs(diff) >= 180.0)
			{
				diff = 360.0 - diff;
			}

			if (diff > prevDiff)
			{
				prevDiff = diff;
				m_fixedSeta = m_setaList[i];
			}
		}
	}



	m_seta = m_fixedSeta;
	CString str;
	double radian = m_fixedSeta * 3.141592 / 180.0;

	int r1 = (int)(100.0 * cos(radian));
	int r2 = (int)(100.0 * sin(radian));
	m_fixedPt.x = (m_circlePt.x + r1);
	m_fixedPt.y = (m_circlePt.y + r2);
	
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
