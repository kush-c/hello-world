package com.kush;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public abstract class TimeAwareMap<K, V> {
    public abstract void put(K key, V val);
    public abstract V get(K key, long timeMillis);

    private static class TimeAwareMapImpl<K, V> extends TimeAwareMap<K, V> {

        private final Map<K, ArrayList<TimedVal<V>>> timedMap = new HashMap<>();

        @Override
        public void put(K key, V val) {
            List<TimedVal<V>> list = timedMap.getOrDefault(key, new ArrayList<>());
            list.add(new TimedVal<>(val, System.currentTimeMillis()));
        }

        @Override
        public V get(K key, long timeMillis) {
            ArrayList<TimedVal<V>> list = timedMap.get(key);
            return list == null ? null : getAtOrBefore(list, timeMillis);
        }

        private V getAtOrBefore(ArrayList<TimedVal<V>> list, long timeMillis) {
            return getAtOrBefore(list, timeMillis, 0, list.size() -1);
        }

        private V getAtOrBefore(ArrayList<TimedVal<V>> list, long timeMillis, int start, int end) {
            int curr = (start + end) /2;
            return null;
        }
    }

    private static class TimedVal<V> {
        private final V val;
        private final long timeMillis;

        TimedVal(V val, long timeMillis) {
            this.val = val;
            this.timeMillis = timeMillis;
        }
    }

}
