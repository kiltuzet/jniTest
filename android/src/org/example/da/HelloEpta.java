package org.example.da;

import android.util.Log;

public class HelloEpta {
    public static void hello() {
        System.out.println("HelloEpta.hello() called from Qt");
        Log.i("HelloEpta", "HelloEpta.hello() called from Qt");
    }

    public static String retres(String input) {
        String out = "Java echo: " + input;
        Log.i("HelloEpta", "retres input=" + input + " -> " + out);
        return out;
    }
}
