package com.github.svieffect.speaker.findColor;

class CollectColorSort {
    protected void sort(CollectColorNode[] nodes) {
        int end = nodes.length - 1;
        while(end > 0) {
            swap(nodes[0], nodes[end]);
            end--;
            siftDown(nodes, 0, end);
        }
    }
    
    protected void makeTree(CollectColorNode[] nodes) {
        int start = (nodes.length - 2) / 2;
        while(start >= 0) {
            siftDown(nodes, start, nodes.length - 1);
            start--;
        }
    }

    protected boolean siftDown(CollectColorNode[] array, int start, int end) {
        int root = start;
        int child;
        int swap;
        
        while(root * 2 + 1 <= end) {
            child = root * 2 + 1;
            swap = root;
            
            if(array[swap].getValue() > array[child].getValue()) {
                swap = child;
            }
            if(child+1 <= end && array[swap].getValue() > array[child+1].getValue()) {
                swap = child + 1;
            }
            if(swap != root) {
                swap(array[swap], array[root]);
                root = swap;
            }
            else 
                return true;
        }
        return false;
    }
    
    protected void swap(CollectColorNode a1, CollectColorNode a2) {
        int temp;
        
        temp = a1.getKey();
        a1.setKey(a2.getKey());
        a2.setKey(temp);
        
        temp = a1.getValue();
        a1.setValue(a2.getValue());
        a2.setValue(temp);
    }
    
    protected void checkTree(CollectColorNode[] nodes) {
        siftDown(nodes, 0, nodes.length-1);
    }
}
