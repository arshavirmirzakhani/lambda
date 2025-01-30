from OdenGraphQt import BaseNode


class ConstantNode(BaseNode):

    __identifier__ = 'Nodes'
    NODE_NAME = 'Constant'

    def __init__(self):
        super(ConstantNode, self).__init__()
        self.add_output('Output')
        self.add_text_input('value_input',label='Value')

class LabelNode(BaseNode):

    __identifier__ = 'Nodes'
    NODE_NAME = 'Label'

    def __init__(self):
        super(LabelNode, self).__init__()
        self.add_input('Value')

class AddNode(BaseNode):

    __identifier__ = 'Nodes'
    NODE_NAME = 'Add'

    def __init__(self):
        super(AddNode, self).__init__()
        self.add_input('A')
        self.add_input('B')
        self.add_output('Result')
