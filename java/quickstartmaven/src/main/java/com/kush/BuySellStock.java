package com.kush;

public class BuySellStock {
    public int maxProfit(int[] prices) {
        if (prices.length < 2) return 0;
        int[][] max = new int[prices.length][prices.length];
        for (int i=0; i< prices.length; i++) {
            for (int j=0; i<prices.length; j++) {
                max[i][j] = -1;
            }
        }
        return maxRange(prices, max, 0, prices.length -1);
    }

    private int maxRange(int[] prices, int[][] max, int start, int end) {
        if (max[start][end] != -1) return max[start][end];

        int rangeSize = end - start;
        if (rangeSize < 2) {
            max[start][end] = 0;
            return 0;
        } else if (rangeSize == 2) {
            int profit = prices[end] - prices[start];
            max[start][end] = profit > 0 ? profit : 0;
            return max[start][end];
        } else if (rangeSize <= 4) {
            // max one transaction possible. Find a elegant way to
        }

        return 0;
    }
}
