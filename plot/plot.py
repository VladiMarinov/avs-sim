import sys
import csv
import matplotlib.pyplot as plt

def main():
    print("Plotting data from ", sys.argv[1], " ...")
    csv_file_path = sys.argv[1]
    
    freq_buffer = []
    val_buffer = []

    with open(csv_file_path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                print(f'Column names are {", ".join(row)}')
                line_count += 1
            else:
                freq_buffer.append(float(row[0]))
                val_buffer.append(float(row[1]))
                line_count += 1
        print(f'Processed {line_count} lines.')

    plt.style.use('seaborn')
    plt.plot(freq_buffer, val_buffer)
    plt.ylabel("dB")
    plt.xlabel("Frequency (Hz)")
    plt.xlabel("Gain  (dB)")
    plt.xscale('log')
    plt.title('Transfer function')
    plt.autoscale(True, 'x', True)
    plt.show()
    
    # Uncomment this next line if u want to save the output as a picture
    #fig.savefig("test.png")

    plt.show()

if __name__ == "__main__":
    
    main()
