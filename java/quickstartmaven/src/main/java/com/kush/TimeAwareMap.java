package com.kush;

public abstract class TimeAwareMap<K, V> {
    public abstract void put(K key, V val);
    public abstract V get(K key, long timeMillis);

    private static class TimeAwareMapImpl<K, V> extends TimeAwareMap<K, V> {
        

        @Override
        public void put(K key, V val) {

        }

        @Override
        public V get(K key, long timeMillis) {
            return null;
        }
    }

}
