#!/usr/bin/env python

class Diff:

    def pathFinder(self):
        self.path1 = input("Original File: ")
        self.path2 = input("Modified File: ")

    def fileStore(self):
        file1 = open(self.path1, 'r')
        file2 = open(self.path2, 'r')

        self.orgFile = file1.read()
        self.modFile = file2.read()

    def convertToBin(self):
        self.binOrgFile = ' '.join(map(bin, bytearray(self.orgFile, 'utf8')))
        self.binModFile = ' '.join(map(bin, bytearray(self.modFile, 'utf8')))

    def diffFinder(self):

        diffCount = 0

        for i, c in enumerate(self.binOrgFile):
            if self.binOrgFile[i] != self.binModFile[i]:
                diffCount += 1

        return diffCount

diff = Diff()

diff.pathFinder()
diff.fileStore()
diff.convertToBin()
print("Differences between files: %d bytes" % diff.diffFinder())
