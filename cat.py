import os

def main():
    fold = open("old")
    fnew = open("new")

    for line in fold:
        iold = map(int, line.split())
        inew = map(int, fnew.readline().split())

        dmin = iold[1] - inew[1]
        dmax = inew[2] - iold[2]

        if dmin > 0:
            f = open("squares/min/" + str(iold[0]))
            f.readline()
            for l in f:
                print l,
            print ";"

        if dmax > 0:
            f = open("squares/max/" + str(iold[0]))
            f.readline()
            for l in f:
                print l,
            print ";"



    fold.close()
    fnew.close()


if __name__ == "__main__":
    main()
