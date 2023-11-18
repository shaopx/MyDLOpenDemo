package com.spx.mydlopendemo

import android.os.Build
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.spx.mydlopendemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    val TAG = "MainActivity"
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        Log.i(TAG, "当前版本:${Build.VERSION.SDK_INT}")
        if (Build.VERSION.SDK_INT != Build.VERSION_CODES.S) {
            // 如果不是 N 版本，显示 Toast 提示
            showToast("这个demo只验证了android 12版本,高于这个版本可能行, 小于这个版本基本不行")
        }
        try {
            val nativePeer: Long = ReflectHelper.findField(Thread::class.java, "nativePeer")
                .getLong(Thread.currentThread())
            val strFromNative = stringFromJNI(nativePeer)
            Log.i(TAG, "onCreate: strFromNative:${strFromNative}")
            binding.sampleText.text = strFromNative
        } catch (e: Exception) {
            Log.i(TAG, "不支持当前android版本")
        }
    }

    /**
     * A native method that is implemented by the 'mydlopendemo' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(self: Long): String

    private fun showToast(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_LONG).show()
    }

    companion object {
        // Used to load the 'mydlopendemo' library on application startup.
        init {
            System.loadLibrary("mydlopendemo")
        }
    }
}