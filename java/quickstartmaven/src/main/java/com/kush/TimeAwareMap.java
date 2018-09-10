package com.kush;

import java.util.*;

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

            int index = Collections.binarySearch(list, new TimedVal(null, timeMillis));
            if (index >= 0) {
                return list.get(index).val;
            }
            if (index == -1) {
                // There was no value inserted at or before given timestamp.
                return null;
            }
            // Index of (insertionPoint -1)
            index = (index + 2) * -1;
            return list.get(index).val;
        }
    }

    private static class TimedVal<V> implements Comparable<TimedVal> {
        private final V val;
        private final long timeMillis;

        TimedVal(V val, long timeMillis) {
            this.val = val;
            this.timeMillis = timeMillis;
        }

        @Override
        public int compareTo(TimedVal other) {
            // cant simply do "return timeMillis - other.timeMillis" as the result is long
            // and we need an int here.
            if (timeMillis == other.timeMillis) {
                return 0;
            } else if (timeMillis < other.timeMillis) {
                return -1;
            }
            return 1;
        }
    }

}

/*
## Submitting Your Solution

Use any language you're comfortable with and feel free to consult any
documentation, StackOverflow, etc. as you normally would in your day-to-day
work.

Pick your desired language from the drop-down above, or, if it isn't supported by CoderPad,
 just let us know and include instructions on how to build and run your solution on recent-ish Linux
  or MacOS systems.

You can develop and run your solution right in CoderPad if you want, or work in your preferred
 environment and paste your code in when you're ready -- just make sure that it does run in
  CoderPad before you're done.

In an effort to keep our interview process anonymous and objective, please do not enter your name
 into the pad and instead enter "Candidate."

Once you're finished, just submit the pad using the provided Greenhouse link. This take home
 should take you between 1 and 2 hours to complete, though please make sure to note about how
  long the exercise took you. If you have any questions, please shoot us an email at
  recruiting@cockroachlabs.com.  Good luck!


## Implementing a Time-Aware Map

For this exercise, create a map data structure that supports key-value
storage and retrieval. The structure should be time-aware, meaning that
it can maintain multiple values at different times for each key.

The structure's API should support the following operations:

- Put(key, value)
- Get(key, timestamp)

The time-aware map should be exposed as a self-contained library. Its
interface should be idiomatic in whatever language you chose to use.

## Retrieval Behavior

When retrieving a value for a key at a given timestamp, the map will
return the "most recent" value for that key. Here, "most recent" means
the value with the largest timestamp less than or equal to that passed
to Get.

Sane behavior should be defined for the cases where a key does not exist
in the map or where no value existed yet for the key at the specified
timestamp.

### Analysis

Above each operation, please leave a comment discussing its computational time
complexity.

## Testing

Here are a few test cases to get you started (though you will certainly want to
add more):
```
put('a', 'v1') @ timestamp=1

get('a', 1) -> 'v1'
get('a', 2) -> 'v1'
get('b', 2) -> None
```
 */