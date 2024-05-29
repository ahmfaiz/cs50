# Sorting Algorithm Properties

## Bubble Sort
### Working:
Compares 2 adjacent places. If they are in correct order, skip, else swap.

### Behaviour:
- Fastest in Sorted list
- Slowest in all other cases

## Selection Sort
On each iteration, selects the smallest elemnt and moves it to the first unsorted position.

### Behaviour:
- Slowest for reversed list but still faster than bubble sort due to less swapping per iteration

## Merge Sort
Divides the list into half recursively until it goes down to elements and then merges them in order.

### Behaviour:
- Best in most cases


# Findings:

## Sort 1
random - 5.15
sorted - 0.008
reversed - 4.38
**bubble**

## Sort 2
random - 0.019
sorted - 0.012
reversed - 0.012
**merge**

## Sort 3
random - 1.92
sorted - 1.87
reversed - 1.97
**selection**
