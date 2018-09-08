package com.kush;

import com.google.common.collect.ImmutableList;

import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import static java.util.stream.Collectors.joining;
import static java.util.stream.Collectors.toList;

public class Test2 {

    private static String STR = "C:\\Users\\ks2d8\\IdeaProjects\\quickstartmaven\\src\\input";

    // Complete the dynamicArray function below.
    static List<Integer> dynamicArray(int n, List<List<Integer>> queries) {
        List<List<Integer>> seqList =
                Stream.generate(ArrayList<Integer>::new)
                        .limit(n)
                        .collect(Collectors.toList());
        int lastAnswer = 0;
        List<Integer> answers = new ArrayList<>();

        for (List<Integer> query : queries) {
            boolean qType1 = query.get(0) == 1 ? true : false;
            int x = query.get(1);
            int y = query.get(2);
            List<Integer> seq = seqList.get((x ^ lastAnswer) % n);
            if (qType1) {
                seq.add(y);
            } else {
                int size = seq.size();
                lastAnswer = seq.get(y % size);
                answers.add(lastAnswer);
            }
        }
        return answers;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader bufferedReader = new BufferedReader(new FileReader(STR));
        BufferedWriter bufferedWriter = new BufferedWriter(new OutputStreamWriter(System.out));

        String[] nq = bufferedReader.readLine().replaceAll("\\s+$", "").split(" ");

        int n = Integer.parseInt(nq[0]);

        int q = Integer.parseInt(nq[1]);

        List<List<Integer>> queries = new ArrayList<>();

        IntStream.range(0, q).forEach(i -> {
            try {
                queries.add(
                        Stream.of(bufferedReader.readLine().replaceAll("\\s+$", "").split(" "))
                                .map(Integer::parseInt)
                                .collect(toList())
                );
            } catch (IOException ex) {
                throw new RuntimeException(ex);
            }
        });

        List<Integer> result = dynamicArray(n, queries);

        bufferedWriter.write(
                result.stream()
                        .map(Object::toString)
                        .collect(joining("\n"))
                        + "\n"
        );

        bufferedReader.close();
        bufferedWriter.close();
    }
}
