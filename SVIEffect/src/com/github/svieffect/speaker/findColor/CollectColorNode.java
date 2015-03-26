package com.github.svieffect.speaker.findColor;


class CollectColorNode {
    private int mKey;
    private int mValue;
    protected void setKey(int key) {
        this.mKey = key;
    }
    protected void setValue(int value) {
        this.mValue = value; 
    }
    protected void setNode(int key, int value) {
        this.mKey = key;
        this.mValue = value;
    }
    protected int getKey() {
        return this.mKey;
    }
    protected int getValue() {
        return this.mValue;
    }
}
