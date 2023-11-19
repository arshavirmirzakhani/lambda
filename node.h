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


struct Value {
    int int_val;
    float float_val;
    std::string str_val;
    bool bool_val;

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
        editor::NodeId node_id;
        Value node_value;
        Node(){}
        virtual void draw(){}
        virtual void run(){}

};

std::vector<Link*>links;
std::vector<Node*>nodes;

editor::PinId get_connected_to_pin(int id){
    int index = 0;

    for (auto link = links.begin();link != links.end(); link++) {
        if ((*link)->begin_id.Get() == id){
            return (*link)->end_id.Get();
        }
        if ((*link)->end_id.Get() == id){
            return (*link)->begin_id.Get();
        }

        index++;
    }
}

void set_value(int id,Value set_value){
    int index = 0;

    for (auto link = links.begin();link != links.end(); link++) {
        if ((*link)->begin_id.Get() == id){
            (*link)->link_value = set_value;
        }

        index++;
    }
}

Value get_value(int id){
    int index = 0;

    for (auto link = links.begin();link != links.end(); link++) {
        if ((*link)->end_id.Get() == id){
            return (*link)->link_value;
        }

        index++;
    }
}


// nodes definition

class add_Node : public Node {
    public:
        editor::PinId IN_pin_1;
        editor::PinId IN_pin_2;
        editor::PinId OUT_pin;       
        int pin_1_val,pin_2_val;


        add_Node(){node_id = UNIQUE_ID++;IN_pin_1 = UNIQUE_ID++;IN_pin_2 = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;}

        void draw(){


            editor::BeginNode(node_id);
            ImGui::Text("Add");

            editor::BeginPin(IN_pin_1,editor::PinKind::Input);
            ImGui::Text("A");
            ImGui::SameLine();

            ImGui::SetNextItemWidth(100);
            ImGui::InputInt("",&pin_1_val);

            editor::EndPin();

            editor::BeginPin(IN_pin_2,editor::PinKind::Input);
            ImGui::Text("B");
            ImGui::SameLine();

            ImGui::SetNextItemWidth(100);
            ImGui::InputInt("",&pin_2_val);

            editor::EndPin();

            editor::BeginPin(OUT_pin,editor::PinKind::Output);
            ImGui::Text("A + B");
            editor::EndPin();

            editor::EndNode();
        }

        void run(){
            node_value.int_val = pin_1_val + pin_2_val;
            set_value(OUT_pin.Get(),node_value);
        }

};

class lable_Node : public Node {
    public:
        int pin_value = 0;
        editor::PinId IN_pin;

        lable_Node(){node_id = UNIQUE_ID++;IN_pin = UNIQUE_ID++;}

        void draw(){
            editor::BeginNode(node_id);
            ImGui::Text("Add");

            editor::BeginPin(IN_pin,editor::PinKind::Input);
            ImGui::Text("Value");
            editor::EndPin();

            pin_value = get_connected_to_pin(IN_pin.Get()).Get();
            ImGui::Text(std::to_string(pin_value).c_str());

            editor::EndNode();
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