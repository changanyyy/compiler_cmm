import json
import os

def main():

    str = "./Tests_3_Advanced"
    path_list = os.listdir(str+"/tests")
    pathtrue_list = os.listdir(str+"/expects")
    path_list.sort()
    ALL_TESTS = True
    for name in path_list:
        if not "cmm" in name: continue
        pathtst=str+"/tests/"+name
        if pathtrue_list.count(name.replace("cmm","output")) == 0:
            pathtrue=str+"/expects/"+name.replace("cmm","out")
        else:
            pathtrue=str+"/expects/"+name.replace("cmm","output")
        
        output = []
        strr = "tests/2021-"+name
        with open(pathtst, "r") as f_test:
            with open(strr,"w") as f:
                f.write(f_test.read())

        with open(pathtrue, 'r') as f_t:
            for linet in f_t.readlines():
                output.append(int(linet))
            output = [[[],output, 0]]
            with open(strr.replace("cmm","json"),"w") as f:
                json.dump(output,f)
        # break
        
            
        
if __name__=="__main__":
    main()