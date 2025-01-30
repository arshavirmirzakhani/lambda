from OdenGraphQt.base.commands import NodeAddedCmd
from OdenGraphQt import NodeGraph,NodeObject
from OdenGraphQt.errors import NodeCreationError

# overrided base class to prevent adding number to node title
# its recommended to not remove or change it unless you know what you are doing

class Graph(NodeGraph):

    def create_node(self, node_type, name=None, selected=True, color=None,
                    text_color=None, pos=None, push_undo=True):
        
        node = self._node_factory.create_node_instance(node_type)
        if node:
            node._graph = self
            node.model._graph_model = self.model

            wid_types = node.model.__dict__.pop('_TEMP_property_widget_types')
            prop_attrs = node.model.__dict__.pop('_TEMP_property_attrs')

            if self.model.get_node_common_properties(node.type_) is None:
                node_attrs = {node.type_: {
                    n: {'widget_type': wt} for n, wt in wid_types.items()
                }}
                for pname, pattrs in prop_attrs.items():
                    node_attrs[node.type_][pname].update(pattrs)
                self.model.set_node_common_properties(node_attrs)

            accept_types = node.model.__dict__.pop(
                '_TEMP_accept_connection_types'
            )
            for ptype, pdata in accept_types.get(node.type_, {}).items():
                for pname, accept_data in pdata.items():
                    for accept_ntype, accept_ndata in accept_data.items():
                        for accept_ptype, accept_pnames in accept_ndata.items():
                            for accept_pname in accept_pnames:
                                self._model.add_port_accept_connection_type(
                                    port_name=pname,
                                    port_type=ptype,
                                    node_type=node.type_,
                                    accept_pname=accept_pname,
                                    accept_ptype=accept_ptype,
                                    accept_ntype=accept_ntype
                                )
            reject_types = node.model.__dict__.pop(
                '_TEMP_reject_connection_types'
            )
            for ptype, pdata in reject_types.get(node.type_, {}).items():
                for pname, reject_data in pdata.items():
                    for reject_ntype, reject_ndata in reject_data.items():
                        for reject_ptype, reject_pnames in reject_ndata.items():
                            for reject_pname in reject_pnames:
                                self._model.add_port_reject_connection_type(
                                    port_name=pname,
                                    port_type=ptype,
                                    node_type=node.type_,
                                    reject_pname=reject_pname,
                                    reject_ptype=reject_ptype,
                                    reject_ntype=reject_ntype
                                )

            node.NODE_NAME = name or node.NODE_NAME
            node.model.name = node.NODE_NAME
            node.model.selected = selected

            def format_color(clr):
                if isinstance(clr, str):
                    clr = clr.strip('#')
                    return tuple(int(clr[i:i + 2], 16) for i in (0, 2, 4))
                return clr

            if color:
                node.model.color = format_color(color)
            if text_color:
                node.model.text_color = format_color(text_color)
            if pos:
                node.model.pos = [float(pos[0]), float(pos[1])]

            node.model.layout_direction = self.layout_direction()

            node.update()

            undo_cmd = NodeAddedCmd(
                self, node, pos=node.model.pos, emit_signal=True
            )
            if push_undo:
                undo_label = 'create node: "{}"'.format(node.NODE_NAME)
                self._undo_stack.beginMacro(undo_label)
                for n in self.selected_nodes():
                    n.set_property('selected', False, push_undo=True)
                self._undo_stack.push(undo_cmd)
                self._undo_stack.endMacro()
            else:
                for n in self.selected_nodes():
                    n.set_property('selected', False, push_undo=False)
                undo_cmd.redo()

            return node

        raise NodeCreationError(f"Can't find node: {node_type}")

    def add_node(self, node, pos=None, selected=True, push_undo=True):
        
        assert isinstance(node, NodeObject), 'node must be a Node instance.'

        wid_types = node.model.__dict__.pop('_TEMP_property_widget_types')
        prop_attrs = node.model.__dict__.pop('_TEMP_property_attrs')

        if self.model.get_node_common_properties(node.type_) is None:
            node_attrs = {node.type_: {
                n: {'widget_type': wt} for n, wt in wid_types.items()
            }}
            for pname, pattrs in prop_attrs.items():
                node_attrs[node.type_][pname].update(pattrs)
            self.model.set_node_common_properties(node_attrs)

        accept_types = node.model.__dict__.pop(
            '_TEMP_accept_connection_types'
        )
        for ptype, pdata in accept_types.get(node.type_, {}).items():
            for pname, accept_data in pdata.items():
                for accept_ntype, accept_ndata in accept_data.items():
                    for accept_ptype, accept_pnames in accept_ndata.items():
                        for accept_pname in accept_pnames:
                            self._model.add_port_accept_connection_type(
                                port_name=pname,
                                port_type=ptype,
                                node_type=node.type_,
                                accept_pname=accept_pname,
                                accept_ptype=accept_ptype,
                                accept_ntype=accept_ntype,
                            )
        reject_types = node.model.__dict__.pop(
            '_TEMP_reject_connection_types'
        )
        for ptype, pdata in reject_types.get(node.type_, {}).items():
            for pname, reject_data in pdata.items():
                for reject_ntype, reject_ndata in reject_data.items():
                    for reject_ptype, reject_pnames in reject_ndata.items():
                        for reject_pname in reject_pnames:
                            self._model.add_port_reject_connection_type(
                                port_name=pname,
                                port_type=ptype,
                                node_type=node.type_,
                                reject_pname=reject_pname,
                                reject_ptype=reject_ptype,
                                reject_ntype=reject_ntype,
                            )

        node._graph = self
        node.NODE_NAME = node.NODE_NAME
        node.model._graph_model = self.model
        node.model.name = node.NODE_NAME

        node.model.layout_direction = self.layout_direction()

        node.update()

        undo_cmd = NodeAddedCmd(self, node, pos=pos, emit_signal=False)
        if push_undo:
            self._undo_stack.beginMacro('add node: "{}"'.format(node.name()))
            self._undo_stack.push(undo_cmd)
            if selected:
                node.set_selected(True)
            self._undo_stack.endMacro()
        else:
            undo_cmd.redo()