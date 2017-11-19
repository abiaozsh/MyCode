
def test(inval):
    
    outval = inval + 1
    def bbb():
        print(outval)
    
    return bbb

fdsa = int(input())
a = test(fdsa)
a()