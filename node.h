#include <string>
#include <vector>
#include <imgui.h>
#include <imgui_node_editor.h>

namespace editor = ax::NodeEditor;


int NODE_ID = 1;
int PIN_ID = 1;
int LINK_ID = 1;

// data structures

enum Node_types {
    ADD,
    LABEL
};

enum Data_types {
    INT_v,
    FLOAT_v,
    STRING_v,
    BOOL_v
};

class Link {

    public:
        editor::LinkId link_id;
        editor::PinId begin_id;
        editor::PinId end_id;

        int int_val;
        float float_val;
        std::string str_val;
        bool bool_val;

        Data_types link_type;

        Link(editor::PinId begin,editor::PinId end){link_id = LINK_ID++;begin_id = begin;end_id = end;}
};

class Node {
    public:
        editor::NodeId node_id;

        Node(){}
        virtual void draw(){}
        virtual void run(){}

};

std::vector<Link*>links;
std::vector<Node*>nodes;

// nodes definition

class add_Node : public Node {
    public:
        editor::PinId IN_pin_1;
        editor::PinId IN_pin_2;
        editor::PinId OUT_pin;       

        add_Node(){node_id = NODE_ID++;IN_pin_1 = PIN_ID++;IN_pin_2 = PIN_ID++;OUT_pin = PIN_ID++;}

        void draw(){
            editor::BeginNode(node_id);
            ImGui::Text("Add");

            editor::BeginPin(IN_pin_1,editor::PinKind::Input);
            ImGui::Text("A");
            editor::EndPin();

            editor::BeginPin(IN_pin_2,editor::PinKind::Input);
            ImGui::Text("B");
            editor::EndPin();

            editor::BeginPin(OUT_pin,editor::PinKind::Output);
            ImGui::Text("A + B");
            editor::EndPin();

            editor::EndNode();
        }

};

class lable_Node : public Node {
    public:
        int IN_pin;

        lable_Node(){node_id = NODE_ID++;IN_pin = PIN_ID++;}

        void draw(){
            editor::BeginNode(node_id);
            ImGui::Text("Add");

            editor::BeginPin(IN_pin,editor::PinKind::Input);
            ImGui::Text("Value");
            editor::EndPin();

            editor::EndNode();
        }

};

void proccess_nodes(){
    for (auto node : nodes){
        node->draw();
    }

    for (auto link : links){
        editor::Link(link->link_id,link->begin_id,link->end_id);
    }
}