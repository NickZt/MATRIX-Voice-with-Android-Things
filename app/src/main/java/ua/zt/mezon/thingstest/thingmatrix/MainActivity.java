package ua.zt.mezon.thingstest.thingmatrix;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.don11995.log.SimpleLog;

import ua.zt.mezon.thingstest.thingmatrix.adapter.ViewPagerAdapter;
import ua.zt.mezon.thingstest.thingmatrix.fragment.FragControls;
import ua.zt.mezon.thingstest.thingmatrix.utils.PermissionManager;
import ua.zt.mezon.thingstest.thingmatrix.vadapters.CustomViewPager;


public class MainActivity extends AppCompatActivity implements ActivityNavigator {

    private static final boolean DEBUG = Config.DEBUG;
    private static final String TAG = "TODEL";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public boolean isPlayed = false;
    public CustomViewPager mainVp;
    boolean isEverloopStart = false;
    TextView mBtn_play_pause;
    private MainActivityPresenter mPresenter;
    private SharedPreferences mSharedPreferences;

    private ViewPagerAdapter vpAdapter;

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        Log.d(TAG, "onActivityResult()  Req = " + requestCode + " || Res = " + resultCode + " || Data = " +
                data);

    }

    private void prefAddFileString(String tmpFn) {
        mSharedPreferences.edit().putString(getString(R.string.file_key), tmpFn).commit();
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SimpleLog.setPrintReferences(true);
        matrixSysInit(16000);
        mPresenter = new MainActivityPresenterImpl(this);

        mSharedPreferences = getSharedPreferences("MyPref", MODE_PRIVATE);
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
            PermissionManager permMan = new PermissionManager(this);

            permMan.maybeRequestPermission(null, PermissionManager.Permissions.READ_EXTERNAL_STORAGE);
//            permMan.maybeRequestPermission(null, PermissionManager.Permissions.RECORD_AUDIO);
        }

        findViewById(R.id.buttonEverloopStart).setOnClickListener(v -> {
            isEverloopStart = !isEverloopStart;
            mPresenter.matrixEverloopOperationStart(0, isEverloopStart);
        });

        initializeFragmentsPager();


    }

    public void buttonEverSetText(String tmp) {
        TextView tv = (TextView) findViewById(R.id.buttonEverloopStart);
        tv.setText(tmp);
    }


    private void initializeFragmentsPager() {
        mainVp = findViewById(R.id.mainVp);
        vpAdapter = new ViewPagerAdapter(getSupportFragmentManager());
        vpAdapter.addFragment(new FragControls(), "FFTFeedBackControl");


        Log.d("TODEL", "initializeFragmentsPager() called " + vpAdapter.getCount());
        vpAdapter.getCount();
        mainVp.setOffscreenPageLimit(vpAdapter.getCount());
        mainVp.setAdapter(vpAdapter);
        mainVp.setScanScroll(true);
    }


    public void internaleffectsSelectNStart(int i, int progress, boolean b, int micsamplingFreq) {
        neffectSelector(i, progress, b, micsamplingFreq);
    }


    public String startEverloop(boolean isEverloopStart) {
        return startEverloopN(isEverloopStart);
    }



    public boolean matrixSysInit(int micsamplingFreq) {
        return matrixInit(micsamplingFreq);
    }

    @Override
    public void startDemo(int i, int progress, boolean b, int micsamplingFreq) {
        mPresenter.matrixDemoOperationStart(i, progress, b, micsamplingFreq);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private native String startEverloopN(boolean isEverloopStart);


    /**
     * MIC_sampling_frequencies[][3] = {
     * {8000, 374, 0},  {12000, 249, 2}, {16000, 186, 4}, {22050, 135, 5},
     * {24000, 124, 5}, {32000, 92, 6},  {44100, 67, 7},  {48000, 61, 8},
     * {96000, 30, 10}, {0, 0, 0}};
     *
     * @param micsamplingFreq
     * @return
     */
    private native boolean matrixInit(int micsamplingFreq);

    private native void neffectSelector(int eff_type, int progress, boolean enable, int micsamplingFreq);

}
