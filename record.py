def main():
    for n in xrange(3, 28):
        print "%d " % (n),
        try:
            f = open("squares/min/" + `n`)

            best = int(f.readline())
            f.close()
            print best,
        except (IOError, OSError) as e:
            print 0,

        print " ",

        try:
            f = open("squares/max/" + `n`)

            best = int(f.readline())
            f.close()
            print best
        except (IOError, OSError) as e:
            print 0


if __name__ == "__main__":
    main()
