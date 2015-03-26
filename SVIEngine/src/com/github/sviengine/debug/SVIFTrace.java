package com.github.sviengine.debug;

/**
 * SAFTrace Class
 * this is for debug
 */
public class SVIFTrace {
	
	/**
	 * enable
	 */
	public static void enable() {
		nativeEnable();
	}
	
	/**
	 * disable
	 */
	public static void disable() {
		nativeDisable();
	}
	
	/**
	 * mark
	 * marking string when debug
	 *
	 * @param markInfo > mark information about string
	 */
	public static void mark(String markInfo) {
		nativeMark(markInfo);
	}
	
	private static native void nativeEnable();
	private static native void nativeDisable();
	private static native void nativeMark(String markInfo);
}
