//
// (C) Copyright 2002-2006 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//
//
#if !defined(AFX_GSPREVIEWCTRL_H__7DC11025_C827_11D2_B89C_080009DCA551__INCLUDED_)
#define AFX_GSPREVIEWCTRL_H__7DC11025_C827_11D2_B89C_080009DCA551__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GsPreviewCtrl.h : header file
//
#include "acgi.h"
#include <math.h>
// Simple lighweight drawable to do orbit glyph
//
class OrbitGadget : public AcGiDrawable
{
  AcGsNode* m_pNode;
  AcGsView* m_pAcGsView;
public:

  OrbitGadget():m_pNode(NULL) {}
  void setGsView(AcGsView* pView) {m_pAcGsView=pView;}

  virtual Adesk::UInt32   subSetAttributes   (AcGiDrawableTraits * traits)
  {
    traits->setTrueColor (AcCmEntityColor(0,200,0));
    return kDrawableNone;
  }
  virtual Adesk::Boolean  subWorldDraw       (AcGiWorldDraw * wd)
  {
    return Adesk::kFalse;
  }
  virtual void            subViewportDraw    (AcGiViewportDraw * pViewportDraw)
  {
    AcGsDCRect view_rect;
    m_pAcGsView->getViewport (view_rect);

    int nViewportX = (view_rect.m_max.x - view_rect.m_min.x) + 1;
    int nViewportY = (view_rect.m_max.y - view_rect.m_min.y) + 1;
    AcGeMatrix3d scale = m_pAcGsView->viewingMatrix() * m_pAcGsView->worldToDeviceMatrix().inverse();
    AcGeVector3d viewDir = m_pAcGsView->target()-m_pAcGsView->position();
    const double radius  = min (nViewportX, nViewportY) * 0.4f*scale.scale();

    pViewportDraw->geometry().circle(m_pAcGsView->target(),radius,viewDir);
    return;
  }
  virtual Adesk::Boolean  isPersistent    (void) const
  { 
    return Adesk::kFalse;
  };                 
  virtual AcDbObjectId    id              (void) const
  {
    return AcDbObjectId::kNull;
  }
};

class ZoomWindowDrawable : public AcGiDrawable
{
    AcGsView* m_pAcGsView;
public:

    ZoomWindowDrawable() {}
    void setGsView(AcGsView* pView) { m_pAcGsView = pView; }

    virtual Adesk::UInt32   subSetAttributes(AcGiDrawableTraits* traits)
    {
        traits->setTrueColor(AcCmEntityColor(200, 0, 0));
        traits->setLinePattern(kDashed);
        return kDrawableNone;
    }
    virtual Adesk::Boolean  subWorldDraw(AcGiWorldDraw* wd)
    {
        return Adesk::kFalse;
    }
    virtual void            subViewportDraw(AcGiViewportDraw* pViewportDraw)
    {
        AcGiViewportGeometry& geom = pViewportDraw->geometry();

        AcGsDCRect view_rect;
        m_pAcGsView->getViewport(view_rect);

        int nViewportX = (view_rect.m_max.x - view_rect.m_min.x) + 1;
        int nViewportY = (view_rect.m_max.y - view_rect.m_min.y) + 1;

        double width  = mEndPoint.x - mStartPoint.x;
        double height = mEndPoint.y - mStartPoint.y;

        // flip y
        mStartPoint.y = nViewportY - 1 - mStartPoint.y;

        AcGePoint3d tl = mStartPoint;
        AcGePoint3d tr(mStartPoint.x + width, mStartPoint.y, 0.0);

        AcGePoint3d br(mStartPoint.x + width, mStartPoint.y - height, 0.0);
        AcGePoint3d bl(mStartPoint.x, mStartPoint.y - height, 0.0);

        geom.pushPositionTransform(kAcGiScreenPosition, AcGePoint2d(0, 0));

        std::vector<AcGePoint3d> pts = { tl, tr, br, bl, tl };
        geom.polyline((Adesk::UInt32)pts.size(), pts.data());

        geom.popModelTransform();
    }
    virtual Adesk::Boolean  isPersistent(void) const
    {
        return Adesk::kFalse;
    };
    virtual AcDbObjectId    id(void) const
    {
        return AcDbObjectId::kNull;
    }

    AcGePoint3d mStartPoint;
    AcGePoint3d mEndPoint;
};

/////////////////////////////////////////////////////////////////////////////
// CGsPreviewCtrl window

class CGsPreviewCtrl : public CStatic
{

public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CGsPreviewCtrl)
  //}}AFX_VIRTUAL


public:
  CGsPreviewCtrl() : CStatic(), 
    mpManager(NULL), mpGraphicsKernel(NULL), mpDevice(NULL), mpView(NULL), 
    mpModel(NULL),mbModelCreated(false),mbPanning(false),mbOrbiting(false) 
  {
    // zooming functionality
    mZooming = false;
    mMouseDown = false;
    mMouseMoving = false;
  } 

  virtual ~CGsPreviewCtrl() {clearAll();}
  void init(HMODULE hRes, bool bCreateModel = true);
  void erasePreview();
  void clearAll();

  AcGsView*   view()      { return mpView; }
  AcGsDevice* device()    { return mpDevice; }
  AcGsModel*  model()     { return mpModel; }
  void setModel(AcGsModel* pModel);
  // draws XOR ink Rectangle, for zooming
  void RubberRectangle(CPoint startPt, CPoint endPt);
  // Generated message map functions
protected:
  //{{AFX_MSG(CGsPreviewCtrl)
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
#if _MSC_VER < 1400
  afx_msg UINT OnNcHitTest(CPoint point);
#else
  afx_msg LRESULT OnNcHitTest(CPoint point);
#endif
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

public:
  HCURSOR mhSavedCursor;
  HCURSOR mhPanCursor;
  HCURSOR mhCrossCursor;
  HCURSOR mhOrbitCursor;
  AcGsManager         *mpManager;
  AcGsGraphicsKernel  *mpGraphicsKernel;
  AcGsDevice          *mpDevice;
  AcGsView            *mpView;
  AcGsModel           *mpModel;
  AcGsModel           *mpGhostModel;
  OrbitGadget         mOrbitGadget;
  ZoomWindowDrawable  mZoomWindowDrawable;
  bool mbModelCreated;
  bool mbPanning;
  bool mbOrbiting;
  CPoint mStartPt;

  // zooming functionality
  bool mZooming;
  bool mMouseDown;
  bool mMouseMoving;
  CPoint mEndPt; 

public:
  // functions for adding and freeing temporary entities, these are entities that exist in the AcGsView but 
  // don't exist in the AcDbDatabase
  // records a temp entity and returns the total number recorded internally
  int addTempEntity(AcDbEntity *ent);
  // frees the temporary memory, returns the total number freed
  int freeTempEntities();
private:
  AcArray<AcDbEntity *> mTempEnts;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSPREVIEWCTRL_H__7DC11025_C827_11D2_B89C_080009DCA551__INCLUDED_)
