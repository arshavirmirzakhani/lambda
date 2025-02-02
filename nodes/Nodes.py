from OdenGraphQt import BaseNode
from nodes.PortFunc import draw_triangle_port

class StartNode(BaseNode):

    __identifier__ = 'Nodes'
    NODE_NAME = 'Start'

    def __init__(self):
        super(StartNode, self).__init__()
        self.add_output('process',multi_output=False,painter_func=draw_triangle_port)
        
    def process(self):
        pass
    
class ConstantNode(BaseNode):

    __identifier__ = 'Nodes'
    NODE_NAME = 'Constant'

    def __init__(self):
        super(ConstantNode, self).__init__()
        self.add_output('Output')
        self.add_text_input('value_input',label='value')
        
    def process(self):
        pass

class LabelNode(BaseNode):

    __identifier__ = 'Nodes'
    NODE_NAME = 'Label'

    def __init__(self):
        super(LabelNode, self).__init__()
        self.add_input('value')

class AddNode(BaseNode):

    __identifier__ = 'Nodes'
    NODE_NAME = 'Add'

    def __init__(self):
        super(AddNode, self).__init__()
        self.add_input('A')
        self.add_input('B')
        self.add_output('result')
