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

// MFCPaintAppView.cpp : implementation of the CMFCPaintAppView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCPaintApp.h"
#endif

#include "MFCPaintAppDoc.h"
#include "MFCPaintAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCPaintAppView

IMPLEMENT_DYNCREATE(CMFCPaintAppView, CView)

BEGIN_MESSAGE_MAP(CMFCPaintAppView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCPaintAppView::OnFilePrintPreview)
	ON_COMMAND(ID_BUTTON_ERASER, &CMFCPaintAppView::OnEraser)
	ON_COMMAND(ID_BUTTON_PEN, &CMFCPaintAppView::OnPen)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CMFCPaintAppView construction/destruction
//CMFCPaintAppView::CMFCPaintAppView() : m_bDrawing(false), m_bFirstDraw(true) {}


CMFCPaintAppView::CMFCPaintAppView() noexcept
{
	// TODO: add construction code here
	m_bDrawing = false;
	m_bFirstDraw = true;
	m_currentTool = Pen;
	m_pApp = (CMFCPaintAppApp*)AfxGetApp();
	m_clrBackground = RGB(255, 255, 0); // Default white
}

CMFCPaintAppView::~CMFCPaintAppView()
{
}

BOOL CMFCPaintAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCPaintAppView drawing

void CMFCPaintAppView::OnDraw(CDC* pDC)
{
	if (m_bFirstDraw) {
		InitializeMemDC();
		m_bFirstDraw = false;
	}

	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_memDC, 0, 0, SRCCOPY);

	//pDC->BitBlt(0, 0, m_bitmap.GetBitmapDimension().cx, m_bitmap.GetBitmapDimension().cy, &m_memDC, 0, 0, SRCCOPY);

	// TODO: add draw code for native data here
}


// CMFCPaintAppView printing
void CMFCPaintAppView::OnPaint()
{
	//CPaintDC dc(this); // Device context for painting

	//CRect rect;
	//GetClientRect(&rect);
	//dc.FillSolidRect(&rect, m_clrBackground);
}

void CMFCPaintAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCPaintAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCPaintAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCPaintAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCPaintAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCPaintAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCPaintAppView diagnostics

#ifdef _DEBUG
void CMFCPaintAppView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCPaintAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCPaintAppDoc* CMFCPaintAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCPaintAppDoc)));
	return (CMFCPaintAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCPaintAppView message handlers
void CMFCPaintAppView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDrawing) {
		CClientDC dc(this);
		CPen pen; // Black line with width 2
		if (m_currentTool == Pen) {
			pen.CreatePen(PS_SOLID, 5, m_pApp->m_ColorReferece /*RGB(255, 0, 0)*/); // Black line with width 2
		}
		else if (m_currentTool == Eraser) {
			pen.CreatePen(PS_SOLID, 100, RGB(255, 255, 255)); // White line with width 10
		}
		dc.SelectObject(&pen);
		dc.MoveTo(m_ptStart);
		dc.LineTo(point);
		m_memDC.SelectObject(&pen);
		m_memDC.MoveTo(m_ptStart);
		m_memDC.LineTo(point);
		m_bDrawing = false;
	}
	CView::OnLButtonUp(nFlags, point);
}

void CMFCPaintAppView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDrawing = true;
	m_ptStart = point;
	CClientDC dc(this);  // 'this' is a pointer to the window
	//dc.TextOut(10, 10, _T("Hello, World!"));
	CView::OnLButtonDown(nFlags, point);
}

void CMFCPaintAppView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDrawing) {
		CClientDC dc(this);
		CPen pen;
		if (m_currentTool == Pen) {
			pen.CreatePen(PS_SOLID, 5, m_pApp->m_ColorReferece); // Black line with width 2
		}
		else if (m_currentTool == Eraser) {
			pen.CreatePen(PS_SOLID, 100, RGB(255, 255, 255)); // White line with width 10
		}

		dc.SelectObject(&pen);
		dc.MoveTo(m_ptStart);
		dc.LineTo(point);
		m_memDC.SelectObject(&pen);
		m_memDC.MoveTo(m_ptStart);
		m_memDC.LineTo(point);
		m_ptStart = point;
	}
	CView::OnMouseMove(nFlags, point);
}

void CMFCPaintAppView::OnEraser()
{

		m_currentTool = Eraser;
}

void CMFCPaintAppView::OnPen()
{

		m_currentTool = Pen;
}


void CMFCPaintAppView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (cx > 0 && cy > 0) {
		CClientDC dc(this);

		// Create a temporary bitmap to hold the current contents
		CBitmap tempBitmap;
		tempBitmap.CreateCompatibleBitmap(&dc, cx, cy);

		// Select the temporary bitmap into a temporary DC
		CDC tempDC;
		tempDC.CreateCompatibleDC(&dc);
		tempDC.SelectObject(&tempBitmap);

		// Copy the contents of the current memory DC to the temporary DC
		CRect rect;
		GetClientRect(&rect);
		tempDC.BitBlt(0, 0, rect.Width(), rect.Height(), &m_memDC, 0, 0, SRCCOPY);

		// Initialize the new memory DC
		InitializeMemDC();

		// Copy the contents of the temporary DC back to the new memory DC
		m_memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &tempDC, 0, 0, SRCCOPY);

		// Clean up the temporary DC
		tempDC.DeleteDC();
		tempBitmap.DeleteObject();

		Invalidate(FALSE);
	}
	CView::OnSize(nType, cx, cy);
}

void CMFCPaintAppView::InitializeMemDC()
{
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	if (m_bitmap.GetSafeHandle()) {
		m_bitmap.DeleteObject();
	}
	if (m_memDC.GetSafeHdc()) {
		m_memDC.DeleteDC();
	}

	m_bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	m_memDC.CreateCompatibleDC(&dc);
	m_memDC.SelectObject(&m_bitmap);
	m_memDC.FillSolidRect(&rect, RGB(255, 255, 255)); // White background
}