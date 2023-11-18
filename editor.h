#include <imgui.h>
#include <imgui_node_editor.h>
#include <iostream>
#include "node.h"

void setup_theme()
{	
    ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(6.0f, 3.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 5.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 1.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 13.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.8799999952316284f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.2800000011920929f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.8999999761581421f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5372549295425415f, 0.3639653623104095f, 0.2549019753932953f, 0.1620000004768372f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.9019607901573181f, 0.5490196347236633f, 0.294117659330368f, 0.7803921699523926f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.9019607901573181f, 0.5490196347236633f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.5793991088867188f, 0.3512648940086365f, 0.1865018457174301f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.9019607901573181f, 0.5490196347236633f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.4699999988079071f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1333333402872086f, 0.1333333402872086f, 0.1333333402872086f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 0.7803921699523926f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1330458968877792f, 0.1330465227365494f, 0.1330472230911255f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2599999904632568f, 0.5899999737739563f, 0.9800000190734863f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.1330458968877792f, 0.1330465227365494f, 0.1330472230911255f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 0.7803921699523926f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.5793991088867188f, 0.3512648940086365f, 0.1865018457174301f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9019607901573181f, 0.5490196347236633f, 0.294117659330368f, 0.7803921699523926f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.9019607901573181f, 0.5490196347236633f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.9999899864196777f, 0.9999899864196777f, 1.0f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.9999899864196777f, 0.9999946355819702f, 1.0f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.9999899864196777f, 0.9999946355819702f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9999899864196777f, 0.9999983906745911f, 1.0f, 0.03999999910593033f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 0.7803921699523926f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1330458968877792f, 0.1330465227365494f, 0.1330472230911255f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06200150027871132f, 0.1308793425559998f, 0.2188841104507446f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.09532316774129868f, 0.1947980225086212f, 0.3218883872032166f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 0.7803921699523926f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.9019607901573181f, 0.5490196347236633f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.2941176295280457f, 0.4715137779712677f, 0.9019607901573181f, 0.7803921699523926f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9019607901573181f, 0.5490196347236633f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.9999899864196777f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}


void run_editor(){

    editor::Begin("Editor", ImVec2(0.0, 0.0f));

    proccess_nodes();


    if (editor::BeginCreate()) {

        editor::PinId startPinId, endPinId;

        if (editor::QueryNewLink(&startPinId, &endPinId)){

            if (startPinId && endPinId){

                editor::AcceptNewItem(); 
                links.push_back(new Link(startPinId,endPinId));

                editor::Link(UNIQUE_ID,startPinId,endPinId);

            }
            else{
                editor::RejectNewItem();
            }
        }


    }
    editor::EndCreate();

    if (editor::BeginDelete()) {

        editor::LinkId deleted_link_id;

        if (editor::QueryDeletedLink(&deleted_link_id)){

            if (editor::AcceptDeletedItem()){

                int index = 0;

                for (auto link = links.begin();link != links.end(); link++) {

                    if ((*link)->link_id == deleted_link_id) {
                        links.erase(link);
                    }

                    index++;
                }
            }
            else{
                editor::RejectDeletedItem();
            }
        }


    }

    editor::EndDelete();

    editor::End();

    if (ImGui::BeginPopupModal("create_node")) {

        if(ImGui::Button("Add")){
            nodes.push_back(new add_Node());
        }

        ImGui::EndPopup();
    }

    ImGui::Begin("toolbar");

    if (ImGui::Button("Create node")) {
        ImGui::OpenPopup("create_node");
    }

    ImGui::End();
}