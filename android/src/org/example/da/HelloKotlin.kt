package org.example.da

import android.util.Log

object HelloKotlin {
    @JvmStatic
    fun f1() {
        println("HelloKotlin called from Qt (Kotlin)")
        Log.i("HelloKotlin", "f1() called from Qt (Kotlin)")
    }

    @JvmStatic
    fun f2(input: String): String {
        val out = "Kotlin echo: $input"
        Log.i("HelloKotlin", "f2() input=$input -> $out")
        return out
    }
}
