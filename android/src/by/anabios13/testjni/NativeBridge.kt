package by.anabios13.testjni

object NativeBridge {
    init { System.loadLibrary("native-lib") }

    external fun add(a: Int, b: Int): Int
    external fun echo(text: String): String
    external fun mayThrow(shouldThrow: Boolean)

    interface KotlinSink { fun onResult(msg: String) }
    external fun callIntoKotlin(sink: KotlinSink, payload: String)
}
