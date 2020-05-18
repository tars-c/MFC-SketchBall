
// SketchView.h: CSketchView 클래스의 인터페이스
//

#pragma once


class CSketchView : public CView
{
protected: // serialization에서만 만들어집니다.
	CSketchView() noexcept;
	DECLARE_DYNCREATE(CSketchView)

// 특성입니다.
public:
	CSketchDoc* GetDocument() const;

// 작업입니다.
public:
	CPoint m_circlePt = { 100, 100 };
	CSize m_step = { 5, 5 };
	BOOL m_clicked = false;
	CRect m_viewRect;

	CPoint m_startPt = { 0, 0 };
	CPoint m_endPt = { 0, 0 };

	const int m_radian = 40;
	double m_seta = 0.0;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CSketchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // SketchView.cpp의 디버그 버전
inline CSketchDoc* CSketchView::GetDocument() const
   { return reinterpret_cast<CSketchDoc*>(m_pDocument); }
#endif

