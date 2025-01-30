import sys
from PySide6.QtGui import QAction, QIcon
from PySide6.QtWidgets import *
from nodes.Nodes import *
from Graph import Graph


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle('Lambda')

        self.node_graph = Graph()
        self.node_graph.register_node(ConstantNode)
        self.node_graph.register_node(LabelNode)
        self.node_graph.register_node(AddNode)

        graph_context_menu = self.node_graph.get_context_menu('graph')
        graph_nodes_menu = graph_context_menu.add_menu('Create Node')

        graph_nodes_menu.add_command('Constant',lambda : self.node_graph.create_node('Nodes.ConstantNode'))
        graph_nodes_menu.add_command('Label',lambda : self.node_graph.create_node('Nodes.LabelNode'))
        graph_nodes_menu.add_command('Add',lambda : self.node_graph.create_node('Nodes.AddNode'))


        node_context_menu = self.node_graph.get_context_menu('nodes')

        node_context_menu.add_command('Delete node',lambda graph,node : graph.remove_node(node),shortcut='Del',node_class=BaseNode)


        menu_bar = self.menuBar()

        file_menu = menu_bar.addMenu('&File')

        file_save_action = QAction(QIcon(),"&Save",self)
        file_save_action.triggered.connect(self.save_graph)

        file_load_action = QAction(QIcon(),"&Load",self)
        file_load_action.triggered.connect(self.load_graph)

        file_menu.addAction(file_save_action)
        file_menu.addAction(file_load_action)


        self.setCentralWidget(self.node_graph.widget)

    def save_graph(self):

        dialog = QFileDialog(self)
        dialog.setFileMode(QFileDialog.FileMode.AnyFile)
        dialog.setAcceptMode(QFileDialog.AcceptMode.AcceptSave)
        dialog.setNameFilter("Lambda project (*.lambda)")

        if dialog.exec():
            self.node_graph.save_session(dialog.selectedFiles()[0])


    def load_graph(self):

        dialog = QFileDialog(self)
        dialog.setFileMode(QFileDialog.FileMode.AnyFile)
        dialog.setAcceptMode(QFileDialog.AcceptMode.AcceptOpen)
        dialog.setNameFilter("Lambda project (*.lambda)")

        if dialog.exec():
            self.node_graph.load_session(dialog.selectedFiles()[0])



if __name__ == '__main__':
    app = QApplication(sys.argv)

    window = MainWindow()
    window.show() 

    app.exec()
