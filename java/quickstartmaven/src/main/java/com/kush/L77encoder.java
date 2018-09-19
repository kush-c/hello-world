import java.io.*;
import java.util.*;

/*
 * To execute Java, please define "static void main" on a class
 * named Solution.
 *
 * If you need more classes, simply define them inline.
 */

class L77encoder {
    public static void main(String[] args) {
        List<L77> encoded = encode("aacaacabcababac");
        for (L77 tuple : encoded) {
            System.out.println(tuple);
        }
        decode(encoded);
    }

    private static class L77 {
        final long off;
        final long len;
        final char c;

        L77(long off, long len, char c) {
            this.off = off;
            this.len = len;
            this.c = c;
        }

        @Override
        public String toString() {
            return "off:" + off + ", len:" + len + ", c:" + c;
        }
    }

    private static final List<L77> encode(String str) {
        List<L77> tuples = new ArrayList<>();

        int maxLen = 0;
        int offset = 0;
        for (int i =0; i< str.length();) {
            System.out.println("i=" + i);
            for (int j=1; j<=i && i+j <= str.length(); j++) {
                System.out.println("i=" + i + "j=" + j);
                String encodedString = str.substring(0,i);
                System.out.println("encoded string:" + encodedString);
                int index = encodedString.indexOf(str.substring(i,i+j));
                if (index != -1) {

                    maxLen = j;
                    offset = index;
                    System.out.println("maxlen = " + maxLen + " index: " + index);
                }
            }
            if ((i+maxLen) == str.length()) {
                maxLen--;
            }
            char endChar = str.charAt(i+maxLen);
            System.out.println("Adding tuple" + offset + maxLen + endChar);
            tuples.add(new L77(offset, maxLen, endChar));
            i += maxLen+1;
            maxLen = 0;
            offset = 0;
        }
        return tuples;
    }

    private static void decode(List<L77> tuples) {
        char[] buff = new char[10000];
        int decodedUpto = 0;
        for (L77 tuple : tuples) {
            if (tuple.len > 0) {
                System.arraycopy(buff, (int) tuple.off, buff, decodedUpto, (int) tuple.len);
                decodedUpto += tuple.len;
            }
            buff[decodedUpto++] = tuple.c;
        }
        for (int i=0; i< decodedUpto; i++) {
            System.out.print(buff[i]);
        }

    }
}
