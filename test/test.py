import os
import subprocess
import filecmp
import time

def run_example(example_avs, i):
    
    print("----------------------------------")
    print("Running example" , i , " - ", example_avs[0], "...")

    cmd = "make run netlist=" + example_avs[0] + " > /dev/null 2>&1 &"
    os.system(cmd)
    time.sleep(3)

    answer_file = "test/answers/" + example_avs[1] + ".txt"
        
    if filecmp.cmp("output.txt", answer_file, False):
        print("    ✔ CORRECT TEST CASE")
        print("----------------------------------")
        return True
    else:
        print("    ✖ INCORRECT TEST CASE! ENDING TEST RUN....")
        print("    ✖ EXAMPLE IS: ", example_avs[0])
        print("DIFF VIEW: ")
        os.system("diff output.txt " + answer_file)
        print("----------------------------------")
        return False



def main():
    
    test_examples_dir = "test/examples/"

    examples = []

    for root, dirs, files in os.walk(test_examples_dir):
        for file in files:
            #  examples_long_name.append(os.path.join(root,file))
            examples.append([os.path.join(root,file), file[0:-4]])
            #  examples_short_name.append(file)

    i = 0

    for example in examples:
        i+=1 
        if not run_example(example, i):
            break


if __name__ == "__main__":

    main()
