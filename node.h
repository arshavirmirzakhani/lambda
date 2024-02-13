#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
#include <imgui.h>
#include <imgui_node_editor.h>

namespace editor = ax::NodeEditor;

int UNIQUE_ID = 1;
bool START = false;

// data structures

enum Node_types {
    ADD,
    LABEL,
};

enum Data_types {
    NUM_v,
    STRING_v,
    BOOL_v,
    EXECUTE_V,
};

enum Pin_type {
    Input,
    Output,
};

class Value {
    public:
        float num_val = 0;
        std::string str_val = "";
        bool bool_val = false;

        bool start = false;

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
        bool deletable = true;

        Node(){}

        virtual void draw(){}
        virtual void run(){}

};

std::vector<Link*>links;
std::vector<Node*>nodes;
std::map<int,Pin_type>pins;

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

class constant_Node : public Node {
    public:
        editor::PinId OUT_pin;

        constant_Node(){node_id = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[OUT_pin.Get()] = Pin_type::Output;}

        void draw(){
            editor::BeginNode(node_id);
            ImGui::Text("Constant");

            editor::BeginPin(OUT_pin,editor::PinKind::Output);

            ImGui::PushID(std::to_string(OUT_pin.Get()).c_str());
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("value",&node_value.num_val);
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

class lable_Node : public Node {
    public:
        editor::PinId IN_pin;

        lable_Node(){node_id = UNIQUE_ID++;IN_pin = UNIQUE_ID++;pins[IN_pin.Get()] = Pin_type::Input;}

        void draw(){
            editor::BeginNode(node_id);
            ImGui::Text("label");

            editor::BeginPin(IN_pin,editor::PinKind::Input);

            if (is_pin_available(IN_pin)){
                node_value = get_link_value(IN_pin);
                ImGui::Text(std::to_string(node_value.num_val).c_str());
            }

            editor::EndPin();


            editor::EndNode();
        }

};

class add_Node : public Node {
    public:
        editor::PinId IN_pin_1;
        editor::PinId IN_pin_2;
        editor::PinId OUT_pin;   

        Value value_1;
        Value value_2;
            

        add_Node(){node_id = UNIQUE_ID++;IN_pin_1 = UNIQUE_ID++;IN_pin_2 = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[IN_pin_1.Get()] = Pin_type::Input;pins[IN_pin_2.Get()] = Pin_type::Input;pins[OUT_pin.Get()] = Pin_type::Output;}

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

            if (is_pin_available(IN_pin_1)){
                value_1.num_val = get_link_value(IN_pin_1).num_val;
            }
            else { value_1.num_val = 0; }

            if (is_pin_available(IN_pin_2)){
                value_2.num_val = get_link_value(IN_pin_2).num_val;
            }
            else { value_2.num_val = 0; }

            node_value.num_val = value_1.num_val + value_2.num_val;

            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }

        }

};

class sub_Node : public Node {
    public:
        editor::PinId IN_pin_1;
        editor::PinId IN_pin_2;
        editor::PinId OUT_pin;   

        Value value_1;
        Value value_2;
            

        sub_Node(){node_id = UNIQUE_ID++;IN_pin_1 = UNIQUE_ID++;IN_pin_2 = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[IN_pin_1.Get()] = Pin_type::Input;pins[IN_pin_2.Get()] = Pin_type::Input;pins[OUT_pin.Get()] = Pin_type::Output;}

        void draw(){


            editor::BeginNode(node_id);
            ImGui::Text("Sub");

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

            if (is_pin_available(IN_pin_1)){
                value_1.num_val = get_link_value(IN_pin_1).num_val;
            }
            else { value_1.num_val = 0; }

            if (is_pin_available(IN_pin_2)){
                value_2.num_val = get_link_value(IN_pin_2).num_val;
            }
            else { value_2.num_val = 0; }

            node_value.num_val = value_1.num_val - value_2.num_val;

            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }

        }

};

class mul_Node : public Node {
    public:
        editor::PinId IN_pin_1;
        editor::PinId IN_pin_2;
        editor::PinId OUT_pin;   

        Value value_1;
        Value value_2;
            

        mul_Node(){node_id = UNIQUE_ID++;IN_pin_1 = UNIQUE_ID++;IN_pin_2 = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[IN_pin_1.Get()] = Pin_type::Input;pins[IN_pin_2.Get()] = Pin_type::Input;pins[OUT_pin.Get()] = Pin_type::Output;}

        void draw(){


            editor::BeginNode(node_id);
            ImGui::Text("Mul");

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

            if (is_pin_available(IN_pin_1)){
                value_1.num_val = get_link_value(IN_pin_1).num_val;
            }
            else { value_1.num_val = 0; }

            if (is_pin_available(IN_pin_2)){
                value_2.num_val = get_link_value(IN_pin_2).num_val;
            }
            else { value_2.num_val = 0; }

            node_value.num_val = value_1.num_val * value_2.num_val;

            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }

        }

};

class div_Node : public Node {
    public:
        editor::PinId IN_pin_1;
        editor::PinId IN_pin_2;
        editor::PinId OUT_pin;   

        Value value_1;
        Value value_2;
            

        div_Node(){node_id = UNIQUE_ID++;IN_pin_1 = UNIQUE_ID++;IN_pin_2 = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[IN_pin_1.Get()] = Pin_type::Input;pins[IN_pin_2.Get()] = Pin_type::Input;pins[OUT_pin.Get()] = Pin_type::Output;}

        void draw(){


            editor::BeginNode(node_id);
            ImGui::Text("Div");

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

            if (is_pin_available(IN_pin_1)){
                value_1.num_val = get_link_value(IN_pin_1).num_val;
            }
            else { value_1.num_val = 0; }

            if (is_pin_available(IN_pin_2)){
                value_2.num_val = get_link_value(IN_pin_2).num_val;
            }
            else { value_2.num_val = 0; }

            node_value.num_val = value_1.num_val / value_2.num_val;

            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }

        }

};

class pow_Node : public Node {
    public:
        editor::PinId IN_pin_1;
        editor::PinId IN_pin_2;
        editor::PinId OUT_pin;   

        Value value_1;
        Value value_2;
            

        pow_Node(){node_id = UNIQUE_ID++;IN_pin_1 = UNIQUE_ID++;IN_pin_2 = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[IN_pin_1.Get()] = Pin_type::Input;pins[IN_pin_2.Get()] = Pin_type::Input;pins[OUT_pin.Get()] = Pin_type::Output;}

        void draw(){


            editor::BeginNode(node_id);
            ImGui::Text("Pow");

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

            if (is_pin_available(IN_pin_1)){
                value_1.num_val = get_link_value(IN_pin_1).num_val;
            }
            else { value_1.num_val = 0; }

            if (is_pin_available(IN_pin_2)){
                value_2.num_val = get_link_value(IN_pin_2).num_val;
            }
            else { value_2.num_val = 0; }

            node_value.num_val = pow(value_1.num_val,value_2.num_val);

            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }

        }

};

class abs_Node : public Node {
    public:
        editor::PinId IN_pin;
        editor::PinId OUT_pin;   

        Value value;
            

        abs_Node(){node_id = UNIQUE_ID++;IN_pin = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[IN_pin.Get()] = Pin_type::Input;pins[OUT_pin.Get()] = Pin_type::Output;}

        void draw(){


            editor::BeginNode(node_id);
            ImGui::Text("Abs");

            ImGui::BeginGroup();
            editor::BeginPin(IN_pin,editor::PinKind::Input);

            ImGui::Text("Number");

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

            if (is_pin_available(IN_pin)){
                value.num_val = get_link_value(IN_pin).num_val;
            }
            else { value.num_val = 0; }

            node_value.num_val = abs(value.num_val);

            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }

        }

};

class sqrt_Node : public Node {
    public:
        editor::PinId IN_pin;
        editor::PinId OUT_pin;   

        Value value;
            

        sqrt_Node(){node_id = UNIQUE_ID++;IN_pin = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[IN_pin.Get()] = Pin_type::Input;pins[OUT_pin.Get()] = Pin_type::Output;}

        void draw(){


            editor::BeginNode(node_id);
            ImGui::Text("Sqrt");

            ImGui::BeginGroup();
            editor::BeginPin(IN_pin,editor::PinKind::Input);

            ImGui::Text("Number");

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

            if (is_pin_available(IN_pin)){
                value.num_val = get_link_value(IN_pin).num_val;
            }
            else { value.num_val = 0; }

            node_value.num_val = sqrt(value.num_val);

            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }

        }

};

class pi_Node : public Node {
    public:
        editor::PinId OUT_pin;           

        pi_Node(){node_id = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[OUT_pin.Get()] = Pin_type::Output;}

        void draw(){

            editor::BeginNode(node_id);
            ImGui::Text("Pi");

            ImGui::BeginGroup();
            editor::BeginPin(OUT_pin,editor::PinKind::Output);
            ImGui::Text("Number");
            editor::EndPin();
            ImGui::EndGroup();

            editor::EndNode();
        }

        void run(){

            node_value.num_val = M_PI;

            if (is_pin_available(OUT_pin)){
                set_link_value(OUT_pin,node_value);
            }

        }

};


class start_Node : public Node {
    public:
        editor::PinId OUT_pin;   

        Value value;

        start_Node(){node_id = UNIQUE_ID++;OUT_pin = UNIQUE_ID++;pins[OUT_pin.Get()] = Pin_type::Output;deletable = false;}

        void draw(){

            editor::BeginNode(node_id);
            ImGui::Text("Start");

            ImGui::BeginGroup();
            editor::BeginPin(OUT_pin,editor::PinKind::Output);
            ImGui::Text("on start");
            editor::EndPin();
            ImGui::EndGroup();

            editor::EndNode();
        }

        void run(){

            node_value.start = START;

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
