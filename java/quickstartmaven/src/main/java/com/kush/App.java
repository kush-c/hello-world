package com.kush;

import com.google.common.collect.ImmutableList;

import java.util.List;

/**
 * Hello world!
 *
 */
public class App {
    public static void main( String[] args ) {
        System.out.println( "hello, world" );
        int i = 1+2;
        System.out.println("hello: " + i);
        List<Integer> list = ImmutableList.of(2, 3);
        list.forEach(elem -> System.out.println("Elem: " + elem));
    }
}
