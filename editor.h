#include <imgui.h>
#include <imgui_node_editor.h>
#include <iostream>
#include "node.h"


void run_editor(){

    editor::Begin("Editor", ImVec2(0.0, 0.0f));

    proccess_nodes();


    if (editor::BeginCreate()) {

        editor::PinId startPinId, endPinId;

        if (editor::QueryNewLink(&startPinId, &endPinId)){

            if (startPinId && endPinId){

                editor::AcceptNewItem(); 
                links.push_back(new Link(startPinId,endPinId));

                editor::Link(LINK_ID,startPinId,endPinId);

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

            }
            else{
                editor::RejectDeletedItem();
            }
        }


    }
    editor::EndDelete();

    editor::End();

}