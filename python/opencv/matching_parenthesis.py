
class Matching_Parenthesis:

    def __init__(self, strings: str) -> None:
        self.valid = ['[', '{', '(', '<', '>', ')', '}', ']']
        self.close = ['>', ')', '}', ']']
        self.open = ['<', '(', '{', '[']
        self.inputs = strings
        self.matchs = dict()
        self.__checkMaching()
    def isValid(self, charater):
        return charater in self.valid

    def __checkMaching(self):
        stack = []
        positions = []
        for i, char in enumerate(list(self.inputs)):
            if not self.isValid(char):
                self.matchs[i] = {"index": i, "match": i, "value": char, "parenthesis": False}
            else:
                if char in self.close:
                    if len(stack) == 0:
                        self.matchs = {}
                        print("Input is invalid")
                        return
                    pos = self.close.index(char)
                    prev_match = stack.pop()
                    last_position = positions.pop()
                    if self.open[pos] != prev_match:
                        self.matchs = {}
                        print("Input is invalid")
                        return
                    self.matchs[i] = {"index": i, "match": last_position, "value": char, "parenthesis": True}
                    self.matchs[last_position] = {"index": last_position, "match": i, "value": prev_match, "parenthesis": True}
                else:
                    positions.append(i) 
                    stack.append(char)
        if len(stack) != 0:
            self.matchs = {}
            print("Input is invalid")
            return
        self.matchs = dict(sorted(self.matchs.items()))
        print(self.__str_result())
        
    def __str_result(self) -> str:
        print("                          Matching Parenthesis")
        # return '\n'.join([ str(v) for k, v in self.matchs.items() if v['parenthesis']])
        return '\n'.join([ str({"index": v['index'], "match": v['match'], "value":v["value"]}) for k, v in self.matchs.items() if v['parenthesis']])

    def __str__(self) -> str:
        return '\n'.join([str(k) + ':  ' + str(v) for k, v in self.matchs.items()])

if __name__ == '__main__':
    Matching_Parenthesis("{La [vie] est () () belle !!<>!}")