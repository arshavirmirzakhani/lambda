#include "imgui/imgui.h"
#include "imgui/imnodes.h"
#include <vector>
#include <map>
#include <cmath>
#include <iostream>

int node_id = 0;
int link_id = 0;
bool started = false;

enum node_type {
    Start,
    Constant,
    Label,
    Add,
    Sub,
    Mul,
    Div,
    Power,
    Sqrt,
    Abs,
    Print,
    For,
};

struct value {
    float num_val = 0;
    bool execute = false;
};

std::vector<std::pair<int,int>>links;
std::vector<int>links_id;
std::map<int,value>values_index;
std::vector<node_type>copied_nodes;

class node {
    public:
        int id;
        value node_value;
        node_type type;
        int output_id;
        bool running = false;
        virtual void draw(){};
        virtual void clear(){};
        virtual void run(){};
};

std::vector<node*>nodes;


class start_node : public node {
    public:
        int output_id;
        start_node(int node_id,int output_pin){id = node_id;output_id = output_pin;type = node_type::Start;}
        void draw(){

            values_index[output_id].execute = false;

            if (started == true) {
                values_index[output_id].execute = true;
            }

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(255, 68, 51, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(255, 114, 102, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(255, 114, 102, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Start");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginOutputAttribute(output_id,3);
            ImGui::Text("on start");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

        }

        void clear(){
            values_index.erase(output_id);
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }
};

class constant_node : public node {
    public:
        int output_id;
        constant_node(int node_id,int output_pin){id = node_id;output_id = output_pin;type = node_type::Constant;}
        void draw(){
            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(68, 51, 255, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(114, 102, 255, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(114, 102, 255, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Constant");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::PushItemWidth(100);
            ImGui::InputFloat("value",&node_value.num_val);
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            values_index[output_id] = node_value;
        }

        void clear(){
            values_index.erase(output_id);
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }
};

class label_node : public node {
    public:
        int input_id;
        label_node(int node_id,int input_pin){id = node_id;input_id = input_pin;type = node_type::Label;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id) {
                    node_value = values_index[links[index].first]; 
                }

            }

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(255, 51, 238, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(255, 102, 243, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(255, 102, 243, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Label");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(input_id);
            ImGui::Text(std::to_string(node_value.num_val).c_str());
            ImNodes::EndInputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

};

class add_node : public node {
    public:
        int input_id_1;
        int input_id_2;
        int output_id;
        value value_1;
        value value_2;
        add_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;type = node_type::Add;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_1) {
                    value_1 = values_index[links[index].first]; 
                }

            }

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_2) {
                    value_2 = values_index[links[index].first]; 
                }

            }

            node_value.num_val = value_1.num_val + value_2.num_val;

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(51, 255, 68, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(102, 255, 114, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(102, 255, 114, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Add");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(input_id_1);
            ImGui::Text("A");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(input_id_2);
            ImGui::Text("B");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::Text("A + B");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            values_index[output_id] = node_value;
            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_1){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_2){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            values_index.erase(output_id);
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

};

class sub_node : public node {
    public:
        int input_id_1;
        int input_id_2;
        int output_id;
        value value_1;
        value value_2;
        sub_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;type = node_type::Sub;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_1) {
                    value_1 = values_index[links[index].first]; 
                }

            }

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_2) {
                    value_2 = values_index[links[index].first]; 
                }

            }

            node_value.num_val = value_1.num_val - value_2.num_val;

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(51, 255, 68, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(102, 255, 114, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(102, 255, 114, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Sub");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(input_id_1);
            ImGui::Text("A");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(input_id_2);
            ImGui::Text("B");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::Text("A - B");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            values_index[output_id] = node_value;
            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_1){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_2){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            values_index.erase(output_id);
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

};

class mul_node : public node {
    public:
        int input_id_1;
        int input_id_2;
        int output_id;
        value value_1;
        value value_2;
        mul_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;type = node_type::Mul;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_1) {
                    value_1 = values_index[links[index].first]; 
                }

            }

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_2) {
                    value_2 = values_index[links[index].first]; 
                }

            }

            node_value.num_val = value_1.num_val * value_2.num_val;

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(51, 255, 68, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(102, 255, 114, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(102, 255, 114, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Mul");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(input_id_1);
            ImGui::Text("A");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(input_id_2);
            ImGui::Text("B");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::Text("A * B");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            values_index[output_id] = node_value;
            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_1){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_2){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            values_index.erase(output_id);
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

};

class div_node : public node {
    public:
        int input_id_1;
        int input_id_2;
        int output_id;
        value value_1;
        value value_2;
        div_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;type = node_type::Div;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_1) {
                    value_1 = values_index[links[index].first]; 
                }

            }

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_2) {
                    value_2 = values_index[links[index].first]; 
                }

            }

            node_value.num_val = value_1.num_val / value_2.num_val;

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(51, 255, 68, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(102, 255, 114, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(102, 255, 114, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Div");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(input_id_1);
            ImGui::Text("A");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(input_id_2);
            ImGui::Text("B");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::Text("A / B");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            values_index[output_id] = node_value;
            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_1){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_2){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            values_index.erase(output_id);
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

};

class power_node : public node {
    public:
        int input_id_1;
        int input_id_2;
        int output_id;
        value value_1;
        value value_2;
        power_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;type = node_type::Power;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_1) {
                    value_1 = values_index[links[index].first]; 
                }

            }

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id_2) {
                    value_2 = values_index[links[index].first]; 
                }

            }

            node_value.num_val = pow(value_1.num_val,(int)value_2.num_val);

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(51, 255, 68, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(102, 255, 114, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(102, 255, 114, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Power");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(input_id_1);
            ImGui::Text("A");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(input_id_2);
            ImGui::Text("B");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::Text("A ^ B");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            values_index[output_id] = node_value;
            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_1){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id_2){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            values_index.erase(output_id);
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

};

class sqrt_node : public node {
    public:
        int input_id;
        int output_id;
        value value;
        sqrt_node(int node_id,int input_pin,int output_pin){id = node_id;input_id = input_pin;output_id = output_pin;type = node_type::Sqrt;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id) {
                    value = values_index[links[index].first]; 
                }

            }

            node_value.num_val = sqrt(value.num_val);

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(51, 255, 68, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(102, 255, 114, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(102, 255, 114, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Sqrt");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(input_id);
            ImGui::Text("A");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::Text("sqrt(A)");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            values_index[output_id] = node_value;
            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            values_index.erase(output_id);
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

};

class abs_node : public node {
    public:
        int input_id;
        int output_id;
        value value;
        abs_node(int node_id,int input_pin,int output_pin){id = node_id;input_id = input_pin;output_id = output_pin;type = node_type::Abs;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id) {
                    value = values_index[links[index].first]; 
                }

            }

            node_value.num_val = fabs(value.num_val);

            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(51, 255, 68, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(102, 255, 114, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(102, 255, 114, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Abs");
            ImNodes::EndNodeTitleBar();

            ImNodes::BeginInputAttribute(input_id);
            ImGui::Text("A");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::Text("|A|");
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            values_index[output_id] = node_value;
            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            values_index.erase(output_id);
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].first == output_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

};

class print_node : public node {
    public:
        int input_id;
        int in_exec_id;
        int out_exec_id;
        value value;
        std::vector<std::string>print_list;
        int selectedItemIndex = -1;
        print_node(int node_id,int input_pin, int in_exec_pin,int out_exec_pin){id = node_id;input_id = input_pin;in_exec_id = in_exec_pin;out_exec_id = out_exec_pin;type = node_type::Print;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id) {
                    value = values_index[links[index].first];
                }

            }

            values_index[out_exec_id].execute = false;

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == in_exec_id) {
                    if (values_index[links[index].first].execute == true) {
                        run();
                        values_index[out_exec_id].execute = true;
                    }; 
                }

            }
       
            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(255, 51, 238, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(255, 102, 243, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(255, 102, 243, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("Print");
            ImNodes::EndNodeTitleBar();
            
            ImNodes::BeginInputAttribute(in_exec_id,3);
            ImGui::Text("execute");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(out_exec_id,3);
            ImGui::Text("execute");
            ImNodes::EndOutputAttribute();

            ImNodes::BeginInputAttribute(input_id);
            ImGui::Text("input");

            ImGui::PushItemWidth(150);
            ImGui::BeginListBox("");
            for (int item = 0;item < print_list.size();item++) {
                ImGui::Text(print_list[item].c_str());
            }

            ImGui::EndListBox();

            if (ImGui::Button("reset")){
                print_list.clear();
            }

            ImNodes::EndInputAttribute();


            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

            node_value.num_val = 0;
        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == in_exec_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

        void run() {
            print_list.push_back(std::to_string(value.num_val));
        }

};

class for_node : public node {
    public:
        int input_id;
        int in_exec_id;
        int in_index_id;
        int out_exec_id_1;
        int out_exec_id_2;
        int output_id;
        value value;
        int count;
        int loop_index = 0;
        for_node(int node_id,int input_pin,int in_exec_pin,int in_index_pin,int out_exec_pin_1,int out_exec_pin_2,int output_pin){id = node_id;input_id = input_pin;in_exec_id = in_exec_pin;in_index_id = in_index_pin;out_exec_id_1 = out_exec_pin_1;out_exec_id_2 = out_exec_pin_2;output_id = output_pin;type = node_type::For;}
        void draw(){

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == input_id) {
                    count = values_index[links[index].first].num_val; 
                }
            }

            values_index[out_exec_id_1].execute = false;   
            values_index[out_exec_id_2].execute = false;    

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == in_index_id) {
                    if (values_index[links[index].first].execute == true) {
                        if (running == true) {
                            run();
                        }
                    }; 
                }
            }

            for (int index = 0 ; index < links.size();index++) {
                if (links[index].second == in_exec_id) {
                    if (values_index[links[index].first].execute == true) {
                        loop_index = 0;
                        run();
                    }; 
                }
            }
       
            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(238, 255, 51, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(243, 255, 102, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(243, 255, 102, 255));

            ImNodes::BeginNode(id);

            ImNodes::BeginNodeTitleBar();
            ImGui::Text("For");
            ImNodes::EndNodeTitleBar();
            
            ImNodes::BeginInputAttribute(in_exec_id,3);
            ImGui::Text("execute");
            ImNodes::EndInputAttribute();

            ImNodes::BeginInputAttribute(in_index_id,3);
            ImGui::Text("procces");
            ImNodes::EndInputAttribute();

            ImNodes::BeginOutputAttribute(out_exec_id_1,3);
            ImGui::Text("loop");
            ImNodes::EndOutputAttribute();

            ImNodes::BeginOutputAttribute(out_exec_id_2,3);
            ImGui::Text("end");
            ImNodes::EndOutputAttribute();

            ImNodes::BeginOutputAttribute(output_id);
            ImGui::Text("index");
            ImNodes::EndOutputAttribute();

            ImNodes::BeginInputAttribute(input_id);
            ImGui::Text("count");
            ImNodes::EndInputAttribute();


            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();

        }

        void clear(){
            std::vector<std::pair<int,int>>connected_links;
            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == input_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == in_exec_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int link = 0 ; link < links.size(); link++){
                if (links[link].second == in_index_id){
                    connected_links.push_back(std::make_pair(links[link].first,links[link].second));
                }
            }

            for (int index = 0 ; index < connected_links.size(); index++){
                for (int link = 0 ; link < links.size();link++){
                    if (links[link] == connected_links[index]) {
                        links.erase(links.begin() + link);
                    }
                }
            }

        }

        void run() {
            if (loop_index < count) {
                loop_index++;
                values_index[out_exec_id_1].execute = true;
                values_index[output_id].num_val = loop_index;
                running = true;
            }

            else {
                loop_index = 0;
                values_index[out_exec_id_1].execute = false;
                values_index[out_exec_id_2].execute = true;
                running = false;
            }
            

        }

};


void run_nodes(){
    
    for (int node = 0;node < nodes.size();node++){
        nodes[node]->draw();
    }

    for (int link = 0; link < links.size(); link++) {
        ImNodes::Link(links_id[link], links[link].first, links[link].second);
    }


}

