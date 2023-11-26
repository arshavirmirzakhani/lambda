#include <string>
#include <vector>
#include <iostream>
#include <imgui.h>
#include <imgui_node_editor.h>

namespace editor = ax::NodeEditor;


int UNIQUE_ID = 1;

// data structures

enum Node_types {
    ADD,
    LABEL
};

enum Data_types {
    INT_v,
    FLOAT_v,
    STRING_v,
    BOOL_v,
    EXECUTE_V
};


class Value {
    public:
        int int_val = 0;
        float float_val = 0;
        std::string str_val = "";
        bool bool_val = false;

        bool execute = false;
};

class Link {

    public:
        editor::LinkId link_id;
        editor::PinId begin_id;
        editor::PinId end_id;

        Value link_value;
        Data_types link_type;

        Link(editor::PinId begin,editor::PinId end){link_id = UNIQUE_ID++;begin_id = begin;end_id = end;}
};

class Node {
    public:
        Value node_value;
        editor::NodeId node_id;
        Node(){}
        virtual void draw(){}
        virtual void run(){}

};

std::vector<Link*>links;
std::vector<Node*>nodes;

bool is_pin_available(editor::PinId pin){
    for (auto link : links){
        if (link->begin_id == pin || link->end_id == pin){
            return true;
        }
    }
    return false;
}

Value get_link_value(editor::PinId pin){
    for (auto link : links){
        if (link->begin_id == pin || link->end_id == pin){
            return link->link_value;
        }
    }   
}

void set_link_value(editor::PinId pin,Value value){
    for (auto link : links){
        if (link->begin_id == pin || link->end_id == pin){
            link->link_value = value;
        }
    }   
}


// nodes definition

class add_Node : public Node {
    public:
        editor::PinId IN_pin_1;
        editor::PinId IN_pin_2;
        editor::PinId OUT_pin;       

        add_Node(){node_id = UNIQUE_ID++;IN_pin_1 = UNIQUE_ID++;IN_pin_2 = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;}

        void draw(){


            editor::BeginNode(node_id);
            ImGui::Text("Add");

            ImGui::BeginGroup();
            editor::BeginPin(IN_pin_1,editor::PinKind::Input);

            ImGui::Text("A");

            editor::EndPin();

            editor::BeginPin(IN_pin_2,editor::PinKind::Input);

            ImGui::Text("B");

            editor::EndPin();
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            editor::BeginPin(OUT_pin,editor::PinKind::Output);
            ImGui::Text("Result");
            editor::EndPin();
            ImGui::EndGroup();

            editor::EndNode();
        }

        void run(){

        }

};

class lable_Node : public Node {
    public:
        editor::PinId IN_pin;

        lable_Node(){node_id = UNIQUE_ID++;IN_pin = UNIQUE_ID++;}

        void draw(){
            editor::BeginNode(node_id);
            ImGui::Text("label");

            editor::BeginPin(IN_pin,editor::PinKind::Input);

            if (is_pin_available(IN_pin)){
                node_value = get_link_value(IN_pin);
                ImGui::Text(std::to_string(node_value.float_val).c_str());
            }

            editor::EndPin();


            editor::EndNode();
        }

};

class constant_Node : public Node {
    public:
        editor::PinId OUT_pin;

        constant_Node(){node_id = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;}

        void draw(){
            editor::BeginNode(node_id);
            ImGui::Text("Constant");

            editor::BeginPin(OUT_pin,editor::PinKind::Output);

            ImGui::PushID(std::to_string(OUT_pin.Get()).c_str());
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("value",&node_value.float_val);
            ImGui::PopID();

            editor::EndPin();

            editor::EndNode();
        }

        void run(){
            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }
        }
};

void proccess_nodes(){
    for (auto node : nodes){
        node->draw();
        node->run();
    }

    for (auto link : links){
        editor::Link(link->link_id,link->begin_id,link->end_id);
    }
}