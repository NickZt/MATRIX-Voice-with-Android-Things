package ua.zt.mezon.thingstest.thingmatrix.fragment;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;

import ua.zt.mezon.thingstest.thingmatrix.MainActivity;


public class BaseFragment extends Fragment {

    public BaseFragment() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getAppActivity().isPlayed) {
            Log.e("Home", "Not Connected");
        } else {
            Log.e("Home", "Connected");
        }
    }


    @Override
    public void onResume() {
        super.onResume();
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
    }

    @Override
    public void onPause() {
        super.onPause();
    }


    public MainActivity getAppActivity() {
        MainActivity mainActivity = (MainActivity) getActivity();
        if (mainActivity == null) {
            throw new NullPointerException();
        } else {
            return (MainActivity) getActivity();
        }
    }

}
