package ua.zt.mezon.thingstest.thingmatrix.vadapters;

import android.content.Context;
import android.support.v4.view.ViewPager;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class CustomViewPager extends ViewPager {

    private boolean scrollable = false;

    public CustomViewPager(Context context) {
        super(context);
    }

    public CustomViewPager(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void setScanScroll(boolean scrollable) {
        this.scrollable = scrollable;
    }


    public void setScrollable(boolean enable) {
        scrollable = enable;
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent event) {
        if (scrollable) {
            return super.onInterceptTouchEvent(event);
        } else {
            return false;
        }
    }

    public boolean onTouchEvent(MotionEvent event) {
        if (scrollable) {
            return super.onTouchEvent(event);
        }
        return false;
    }

    public boolean onTouch(View v, MotionEvent event) {
        // Auto-generated method stub

        return false;
    }
}