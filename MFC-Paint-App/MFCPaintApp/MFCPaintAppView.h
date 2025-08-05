// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MFCPaintAppView.h : interface of the CMFCPaintAppView class
//

#pragma once

enum Tool
{
	Pen,
	Eraser
};

class CMFCPaintAppView : public CView
{
protected: // create from serialization only
	CMFCPaintAppView() noexcept;
	DECLARE_DYNCREATE(CMFCPaintAppView)

// Attributes
public:
	CMFCPaintAppDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnPen();
	void OnEraser();
	void ColorPicker();
	void InitializeMemDC();
	void UpdateMemDC();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMFCPaintAppView();
	bool m_bDrawing;
	CPoint m_ptStart;
	CPoint m_ptEnd;
	CBitmap m_bitmap;
	CDC m_memDC;
	bool m_bFirstDraw;
	Tool m_currentTool;
	COLORREF m_ColorRef;
	CMFCPaintAppApp* m_pApp;
	COLORREF m_clrBackground;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	
};

#ifndef _DEBUG  // debug version in MFCPaintAppView.cpp
inline CMFCPaintAppDoc* CMFCPaintAppView::GetDocument() const
   { return reinterpret_cast<CMFCPaintAppDoc*>(m_pDocument); }
#endif

