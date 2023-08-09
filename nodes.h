#include "imgui/imgui.h"
#include "imgui/imnodes.h"
#include <vector>
#include <map>
#include <cmath>


int node_id = 0;
int link_id = 0;


enum node_type {
    constant,
    label,
    add,
    sub
};

struct value {
    float num_val;
};

std::vector<std::pair<int,int>>links;
std::vector<int>links_id;
std::map<int,value>values_index;



class node {
    public:
        int id;
        value node_value;
        virtual void draw(){};
        virtual void clear(){};
};

class constant_node : public node {
    public:
        int output_id;
        constant_node(int node_id,int output_pin){id = node_id;output_id = output_pin;}
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
        label_node(int node_id,int input_pin){id = node_id;input_id = input_pin;}
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
        add_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;}
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
        sub_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;}
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
        mul_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;}
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
        div_node(int node_id,int input_pin_1,int input_pin_2,int output_pin){id = node_id;input_id_1 = input_pin_1;input_id_2 = input_pin_2;output_id = output_pin;}
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



std::vector<node*>nodes;


void run_nodes(){
    
    for (int node = 0;node < nodes.size();node++){
        nodes[node]->draw();
    }

    for (int link = 0; link < links.size(); link++) {
        ImNodes::Link(links_id[link], links[link].first, links[link].second);
    }


}