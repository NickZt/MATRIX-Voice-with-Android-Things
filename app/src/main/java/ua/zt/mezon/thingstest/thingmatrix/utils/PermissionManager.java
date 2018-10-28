package ua.zt.mezon.thingstest.thingmatrix.utils;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.support.v4.app.ActivityCompat;
import android.util.Log;


public class PermissionManager {

    // Add ID's for any additional permissions here, to use them in your Acitivity's
    // onRequestPermissionsResult()
    public static final int READ_EXTERNAL_STORAGE_ID = 0;
    public static final int RECORD_AUDIO_ID = 1;
    private static final String TAG = "PermissionManager";
    /**
     * Checks whether it is necessary to ask for permission. If necessary, it also
     * requests permission.
     *
     * @return true if a permission request is made. False if it is not necessary.
     */
    private Activity _act;

    public PermissionManager(Activity act) {
        _act = act;
    }

    @TargetApi(23)
    public boolean maybeRequestPermission(Uri contentUri, Permissions permission) {

        if (requiresPermission(permission.getPerm())) {
            ActivityCompat.requestPermissions(_act, new String[]{permission.getPerm()}, permission.getId());
            return true;
        } else {
            Log.d(TAG, "Permission for URI: " + contentUri + " & permission: " + permission + " not required " +
                    "or unattainable");
            return false;
        }
    }

    @TargetApi(23)

    public boolean requiresPermission(String permission) {


        boolean isPermissionGranted = false;

        isPermissionGranted = _act.checkSelfPermission(permission) == PackageManager.PERMISSION_GRANTED;


        Log.d(TAG, "isPermissionGranted = " + isPermissionGranted + " for permission:" + permission);
        return !isPermissionGranted;
    }

    public enum Permissions {
        READ_EXTERNAL_STORAGE(Manifest.permission.READ_EXTERNAL_STORAGE, READ_EXTERNAL_STORAGE_ID),
        RECORD_AUDIO(Manifest.permission.RECORD_AUDIO, RECORD_AUDIO_ID);

        private final String perm;
        private final int id;

        Permissions(String perm, int id) {
            this.perm = perm;
            this.id = id;
        }

        public static Permissions match(int requestId) {
            for (Permissions perm : Permissions.values()) {
                if (requestId == perm.getId()) {
                    return perm;
                }
            }
            return null;
        }

        public final int getId() {
            return id;
        }

        public final String getPerm() {
            return perm;
        }
    }

}
