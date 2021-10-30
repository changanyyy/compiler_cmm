import os
import argparse
from utils.logger import setup_logger

def main():
    parser = argparse.ArgumentParser(description="Lab-2 testCode")
    parser.add_argument(
        "--type",
        default=1,
        help="which testcase it should apply(0->Normal,other->Advance)",
        type=int,
    )
    args = parser.parse_args()
    if args.type == 0:
        str = "../Tests_2_Normal/Tests"
    else:
        str = "../Tests_2_Advanced/Tests"
    logger = setup_logger("test", "Result/")

    logger.info(f"now begin to test the {str[11:]} testcase...")
    path_list = os.listdir(str+"/tests")
    pathtrue_list = os.listdir(str+"/expects")
    path_list.sort()
    ALL_TESTS = True
    for name in path_list:
        if not "cmm" in name: continue
        pathtst=str+"/tests/"+name
        if pathtrue_list.count(name.replace("cmm","output")) == 0:
            pathtrue=str+"/expects/"+name.replace("cmm","res")
        else:
            pathtrue=str+"/expects/"+name.replace("cmm","output")
        pathres=str+"/res/"+name.replace("cmm","res")
        os.system("../parser "+pathtst +" > "+pathres)


        with open(pathtrue, 'r',encoding ='utf-8') as f_t,\
            open(pathres, 'r',encoding ='utf-8') as f_r:
            flag = True
            t_iter=iter(f_t.readlines())
            for liner in f_r.readlines():
                try:
                    linet=next(t_iter)
                except StopIteration:
                    flag = False
                    break
                linet = linet.split(':')[0].strip().replace(" ", "").lower()
                a =bytes(linet, encoding ='utf-8')
                liner = liner.split(':')[0].strip().replace(" ", "").lower()
                b =bytes(liner, encoding ='utf-8')
                if (a != b):
                    flag = False
                    break
                
            try:
                linet = next(t_iter)
                if flag == True:
                    flag = False
                    logger.info(f"比答案行数少{linet.strip()}\n")
            except StopIteration:
                pass
            if flag == True:
                logger.info(f"Test for {name}: correct!")
                pass
            else:
                logger.error(f"Test for {name}: error!")
                ALL_TESTS = False
    if ALL_TESTS:
        logger.info(f"Congratulations!<All TestS Passed!>")
            
        
if __name__=="__main__":
    main()