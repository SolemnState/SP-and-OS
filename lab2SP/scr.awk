BEGIN {print "Printing reversed string if more than 10 symbols ";
        FS=" ";
        name=FILENAME;
        filecount=0;
        nOfWords=0;
        nOfStrings=0;
        nOfSymbols=0;
        }
    function showStats() {
        printf ("Number of files: %d\n",filecount);
        printf ("Number of words: %d\n",nOfWords);
        printf ("Number of strings: %d\n",nOfStrings);
        printf ("Number of symbols: %d\n",nOfSymbols);
    }
    {
    if (name!=FILENAME)
    {
        filecount++;
        name=FILENAME;
    }
    nOfSymbols+=length;
    nOfWords+=NF;
    nOfStrings+=1;
    if (length>=10) {
        for (i=NF;i>=1;i--) {
            printf("%s ", $i);
        }
        printf("\n");
    }
}
END	{
    showStats();
    print "END"
}