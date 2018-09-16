import java.io.*;
import java.util.*;
import org.junit.*;
import org.junit.runner.*;

/*
 * To execute Java, please define "static void main" on a class
 * named Solution.
 *
 * If you need more classes, simply define them inline.
 */

public class Solution {
  @Test
  public void testEmptyMap() {
    TimeAwareMap<String, String> timeAwareMap = new TimeAwareMap<>();
    Assert.assertNull(timeAwareMap.get("a", System.currentTimeMillis()));
  }
  
  @Test
  public void testBasicFunctionality() {
    TimeAwareMap<String, String> timeAwareMap = new TimeAwareMap<>();
    long ts1 = timeAwareMap.put("a", "v1");
    Assert.assertEquals("v1", timeAwareMap.get("a", ts1));
    Assert.assertEquals("v1", timeAwareMap.get("a", ts1+1));
    Assert.assertNull(timeAwareMap.get("b", ts1));
  }
  
  @Test
  public void testMiddleTimestamp() throws InterruptedException {
    TimeAwareMap<String, String> timeAwareMap = new TimeAwareMap<>();
    long ts1 = timeAwareMap.put("a", "v1");
    Assert.assertEquals("v1", timeAwareMap.get("a", ts1));
    Assert.assertEquals("v1", timeAwareMap.get("a", ts1*2));
    Thread.sleep(10);
    long ts2 = timeAwareMap.put("a", "v2");
    Assert.assertEquals("v2", timeAwareMap.get("a", ts2));
    Assert.assertEquals("v1", timeAwareMap.get("a", (ts1+ts2)/2));
    Assert.assertEquals("v1", timeAwareMap.get("a", ts2 -1));
  }
                        
  @Test
  public void testHighTimestampValueChanged() throws InterruptedException {
    TimeAwareMap<String, String> timeAwareMap = new TimeAwareMap<>();
    long ts1 = timeAwareMap.put("a", "v1");
    Assert.assertEquals("v1", timeAwareMap.get("a", ts1));
    Assert.assertEquals("v1", timeAwareMap.get("a", ts1*2));
    Thread.sleep(10);

    long ts2 = timeAwareMap.put("a", "v2");
    Assert.assertEquals("v2", timeAwareMap.get("a", ts2));
    Assert.assertEquals("v2", timeAwareMap.get("a", ts1*2));
  }
  
  @Test
  public void testNullAtLowerTimestamp() throws InterruptedException {
    TimeAwareMap<String, String> timeAwareMap = new TimeAwareMap<>();
    long ts1 = timeAwareMap.put("a", "v1");
    Assert.assertEquals("v1", timeAwareMap.get("a", ts1));
    Assert.assertNull(timeAwareMap.get("a", ts1-1));
  }
  
  @Test
  public void testMultipleKeysAndValues() throws InterruptedException {
    TimeAwareMap<String, String> timeAwareMap = new TimeAwareMap<>();
    long tsa1 = timeAwareMap.put("a", "v1");
    long tsb1 = timeAwareMap.put("b", "b1");
    Thread.sleep(10);
    long tsa2 = timeAwareMap.put("a", "v2");
    long tsb2 = timeAwareMap.put("b", "b2");
    Thread.sleep(10);
    long tsa3 = timeAwareMap.put("a", "v3");
    
    Assert.assertNull(timeAwareMap.get("a", tsa1-1));
    Assert.assertEquals("v1", timeAwareMap.get("a", tsa1));
    Assert.assertEquals("v1", timeAwareMap.get("a", tsa1+1));
    Assert.assertEquals("v1", timeAwareMap.get("a", tsa2-1));
    Assert.assertEquals("v2", timeAwareMap.get("a", tsa2));
    Assert.assertEquals("v2", timeAwareMap.get("a", tsa2+1));
    Assert.assertEquals("v2", timeAwareMap.get("a", tsa3-1));
    Assert.assertEquals("v3", timeAwareMap.get("a", tsa3));
    Assert.assertEquals("v3", timeAwareMap.get("a", tsa3+1));
    
    Assert.assertNull(timeAwareMap.get("b", tsb1-1));
    Assert.assertEquals("b1", timeAwareMap.get("b", tsb1));
    Assert.assertEquals("b1", timeAwareMap.get("b", tsb1+1));
    Assert.assertEquals("b1", timeAwareMap.get("b", tsb2-1));
    Assert.assertEquals("b2", timeAwareMap.get("b", tsb2));
    Assert.assertEquals("b2", timeAwareMap.get("b", tsb2+1));
    
    Assert.assertNull(timeAwareMap.get("c", tsa3));    
  }
                        
  

  public static void main(String[] args) {
    JUnitCore.main("Solution");
  }


  /**
   * Implements a Time-aware map. This class is NOT thread safe.
   * @param <K>
   * @param <V>
   */
  private static final class TimeAwareMap<K, V> {

    private final Map<K, ArrayList<TimedVal<V>>> timedMap = new HashMap<>();

    /**
     * Inserts a key/value pair into the map at the current timestamp, and returns the timestamp
     * the value was inserted at.
     * <p>
     * The insertion is done in O(1) time, since the current API only permits insertion at the current
     * timestamp which increases monotonically, hence we can maintain a list of values for the same key
     * sorted in O(1) time.
     * <br>
     * If the API permitted adding values at arbitrary timestamps, we would have to use a balanced tree
     * to keep the sorted values for a key, increasing the time complexity for insertion to O(log V),
     * V being the count of Values for a given key.
     * @param key
     * @param val
     * @return the timestamp (in milliseconds since epoch), at which the value was inserted.
     * (Technically this is the time at which the timestamp was recorded, right before the insertion).
     * @throws NullPointerException if either key/value is null.
     * @throws ClassCastException
     */
    public long put(K key, V val) {
      if (key == null || val == null) {
        throw new NullPointerException();
      }
      ArrayList<TimedVal<V>> list = timedMap.get(key);
      if (list == null) {
        list = new ArrayList<>();
        timedMap.put(key, list);
      };
      long insertionTimestamp = System.currentTimeMillis();
      list.add(new TimedVal<>(val, insertionTimestamp));
      return insertionTimestamp;
    }

    /**
     * Returns the value added at or before the given timestamp for the given key.
     * This method runs in O(log V) where V is the number of values for the given key.
     * As an optimization we check the given timestamp is greater then timestamp of the latest value, 
     * before searching, so we can return in constant time in those cases.
     * @param key
     * @param timeMillis
     * @return the value inserted at or before the given timestamp for the given key. Returns null
     * if no values were inserted before the timestamp for the given key.
     * @throws NullPointerException if the key is null
     * @throws ClassCastException
     */
    public V get(K key, long timeMillis) {
      if (key == null) {
        throw new NullPointerException();
      }

      ArrayList<TimedVal<V>> list = timedMap.get(key);
      if (list == null) {
        return null;
      }

      // Somewhat premature optimization added assuming a bulk of timestamps may be
      // the current timestamp which will be above the time of last put operation.
      TimedVal<V> lastElement = list.get(list.size() - 1);
      if (lastElement.timeMillis <= timeMillis) {
        return lastElement.val;
      }

      int index = Collections.binarySearch(list, new TimedVal<V>(null, timeMillis));
      if (index >= 0) {
        return list.get(index).val;
      }
      if (index == -1) {
        // There was no value inserted at or before given timestamp.
        return null;
      }
      // Binary search has returned (-(insertionPoint) -1)
      // Index of (insertionPoint -1), which is the element with highest timestamp lower than the 
      // given timestamp.
      index = (index + 2) * -1;
      return list.get(index).val;
    }
  }

  private static class TimedVal<V> implements Comparable<TimedVal<V>> {
    private final V val;
    private final long timeMillis;

    TimedVal(V val, long timeMillis) {
      this.val = val;
      this.timeMillis = timeMillis;
    }

    @Override
    public int compareTo(TimedVal<V> other) {
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
Your previous Plain Text content is preserved below:

## Submitting Your Solution

Use any language you're comfortable with and feel free to consult any
documentation, StackOverflow, etc. as you normally would in your day-to-day
work.

Pick your desired language from the drop-down above, or, if it isn't supported by CoderPad, just let us know and include instructions on how to build and run your solution on recent-ish Linux or MacOS systems.

You can develop and run your solution right in CoderPad if you want, or work in your preferred environment and paste your code in when you're ready -- just make sure that it does run in CoderPad before you're done. 

In an effort to keep our interview process anonymous and objective, please do not enter your name into the pad and instead enter "Candidate."

Once you're finished, just submit the pad using the provided Greenhouse link. This take home should take you between 1 and 2 hours to complete, though please make sure to note about how long the exercise took you. If you have any questions, please shoot us an email at recruiting@cockroachlabs.com.  Good luck!


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
