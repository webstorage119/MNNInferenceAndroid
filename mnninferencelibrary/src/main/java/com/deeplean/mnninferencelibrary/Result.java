package com.deeplean.mnninferencelibrary;

public class Result {
    private int index;
    private double score;

    public Result(double score, int index){
        super();
        this.index = index;
        this.score = score;

    }

    public void setClassName(int index){
        this.index = index;

    }

    public void setScore(double score){
        this.score = score;
    }

    public int getIndex(){
        return this.index;
    }

    public double getScore(){
        return this.score;
    }




}
