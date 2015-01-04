def main():
    fold = open("old")
    fnew = open("new")

    for line in fold:
        iold = map(int, line.split())
        inew = map(int, fnew.readline().split())

        dmin = iold[1] - inew[1]
        dmax = inew[2] - iold[2]

        print "%d: %d, %d" % (iold[0], dmin, dmax)

    fold.close()
    fnew.close()


if __name__ == "__main__":
    main()
