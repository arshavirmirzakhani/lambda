#include "imgui/imgui.h"
#include "imgui/imnodes.h"
#include "nodes.h"


int base_node_id = 0;
int base_pin_id = 0;
int base_link_id = 0;

void run_editor() {
 


    ImNodes::BeginNodeEditor();

	run_nodes();

    ImNodes::MiniMap();
    ImNodes::EndNodeEditor();


    ImGui::Begin("nodes");

    if (ImGui::Button("Constant")){
        nodes.push_back(new constant_node(base_node_id++,base_pin_id++));
    }

    if (ImGui::Button("Label")){
        nodes.push_back(new label_node(base_node_id++,base_pin_id++));
    }
    if (ImGui::Button("Add")){
        nodes.push_back(new add_node(base_node_id++,base_pin_id++,base_pin_id++,base_pin_id++));
    }
    if (ImGui::Button("Sub")){
        nodes.push_back(new sub_node(base_node_id++,base_pin_id++,base_pin_id++,base_pin_id++));
    }
    if (ImGui::Button("Mul")){
        nodes.push_back(new mul_node(base_node_id++,base_pin_id++,base_pin_id++,base_pin_id++));
    }
    if (ImGui::Button("Div")){
        nodes.push_back(new div_node(base_node_id++,base_pin_id++,base_pin_id++,base_pin_id++));
    }

    ImGui::End();

    int begin, end;
    if (ImNodes::IsLinkCreated(&begin, &end)) {
        bool is_pin_connected = false;

        for (int link = 0;link < links.size();link++) {
            if (links[link].second == end) {
                is_pin_connected = true;
            }
        }

        if (!is_pin_connected) {
            links.push_back(std::make_pair(begin,end));
            links_id.push_back(base_link_id++);
        }

    }
}

void delete_selected() {

    std::vector<int>selected_nodes;

    for (int node = 0 ; node < nodes.size();node++) {
        if (ImNodes::IsNodeSelected(nodes[node]->id)) {
            selected_nodes.push_back(nodes[node]->id);
        }
    }
    

    for (int selected = 0; selected < selected_nodes.size();selected++) {
        for (int node = 0; node < nodes.size();node++) {
            if (nodes[node]->id == selected_nodes[selected]) {
                nodes[node]->clear();
                nodes.erase(nodes.begin() + node);
            }
        }
    }

    std::vector<int>selected_links;

    for (int link = 0 ; link < links_id.size();link++) {
        if (ImNodes::IsLinkSelected(links_id[link])) {
            selected_links.push_back(links_id[link]);
        }
    }
    

    for (int selected = 0; selected < selected_links.size();selected++) {
        for (int link = 0; link < links.size();link++) {
            if (links_id[link] == selected_links[selected]) {
                links_id.erase(links_id.begin() + link);
                links.erase(links.begin() + link);
            }
        }
    }


}   
