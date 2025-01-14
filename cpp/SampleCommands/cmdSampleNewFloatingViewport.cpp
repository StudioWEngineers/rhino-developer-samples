#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleNewFloatingViewport command
//

#pragma region SampleNewFloatingViewport command

class CCommandSampleNewFloatingViewport : public CRhinoCommand
{
public:
  CCommandSampleNewFloatingViewport() = default;
  ~CCommandSampleNewFloatingViewport() = default;
  UUID CommandUUID() override
  {
    // {55850077-8755-4054-AA92-6A925889A2A3}
    static const GUID SampleNewFloatingViewportCommand_UUID =
    { 0x55850077, 0x8755, 0x4054, { 0xAA, 0x92, 0x6A, 0x92, 0x58, 0x89, 0xA2, 0xA3 } };
    return SampleNewFloatingViewportCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleNewFloatingViewport"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override ;
};

// The one and only CCommandSampleNewFloatingViewport object
static class CCommandSampleNewFloatingViewport theSampleNewFloatingViewportCommand;

CRhinoCommand::result CCommandSampleNewFloatingViewport::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoDoc* doc = context.Document();
  if (nullptr == doc)
    return CRhinoCommand::failure;

  ON_SimpleArray<ON_UUID> viewport_ids;
  ON_SimpleArray<CRhinoView*> view_list;
  CRhinoView* view = nullptr;
  int i = 0;

  // Build a list of (current) viewport ids
  doc->GetViewList(view_list, CRhinoView::ViewTypeFilter::Model);
  for (i = 0; i < view_list.Count(); i++)
  {
    view = view_list[i];
    if (view)
      viewport_ids.Append(view->ActiveViewportID());
  }
  view_list.Empty();

  // Create a new view
  doc->NewView(ON_3dmView(), true);

  // Find the viewport (id) that was just created
  doc->GetViewList(view_list, CRhinoView::ViewTypeFilter::Model);
  for (i = 0; i < view_list.Count(); i++)
  {
    view = view_list[i];
    if (view)
    {
      int rc = viewport_ids.Search(view->ActiveViewportID());
      if (rc < 0)
        break;
      else
        view = 0;
    }
  }

  // Make any necessary updates
  if (view)
  {
    ON_3dmView v = view->ActiveViewport().View();
    v.m_name = L"New View";
    view->ActiveViewport().SetView(v);
    view->FloatRhinoView(true);
    view->Redraw();
  }

  return CRhinoCommand::success;  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleNewFloatingViewport command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
