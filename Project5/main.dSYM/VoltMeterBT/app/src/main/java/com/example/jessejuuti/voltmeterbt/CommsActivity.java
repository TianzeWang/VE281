package com.example.jessejuuti.voltmeterbt;


import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.util.UUID;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class CommsActivity extends AppCompatActivity {

    public BluetoothAdapter BTAdapter = BluetoothAdapter.getDefaultAdapter();
    private static final String TAG = "CommsActivity";
    BluetoothSocket mmSocket;
    BluetoothDevice mmDevice;

    public class ConnectThread extends Thread {
        private ConnectThread(BluetoothDevice device, String msg) throws IOException {
//            if (mmSocket != null) {
//                if (mmSocket.isConnected()) {
//                    Log.e(TAG, "Connected");
//                }
//            }
            BluetoothSocket tmp = null;
            mmDevice = device;
            try {
                UUID uuid = UUID.fromString("94f39d29-7d6d-437d-973b-fba39e49d4ee");
                tmp = mmDevice.createRfcommSocketToServiceRecord(uuid);
            } catch (IOException e) {
                Log.e(TAG, "Socket's create() method failed", e);
            }
            mmSocket = tmp;
            BTAdapter.cancelDiscovery();
            try {
                mmSocket.connect();
                Toast.makeText(getApplicationContext(), "Connection Success",Toast.LENGTH_SHORT).show();
            } catch (IOException connectException) {
                Log.v(TAG, "Connection exception!");
                try {
                    mmSocket.close();
                    /*mmSocket = (BluetoothSocket) mmDevice.getClass().getMethod("createRfcommSocket", new Class[]{int.class}).invoke(mmDevice, 1);
                    mmSocket.connect();
                } catch (NoSuchMethodException e) {
                    e.printStackTrace();
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                } catch (InvocationTargetException e) {
                    e.printStackTrace();
                } */
                } catch (IOException closeException) {

                }
            }
            send(msg);

            TextView voltageLevel = (TextView) findViewById(R.id.Voltage);
            if (msg == "new") {voltageLevel.setText("Data Refreshed");}
        }

        public void send(String msg) throws IOException {
            OutputStream mmOutputStream = mmSocket.getOutputStream();
            mmOutputStream.write(msg.getBytes());
            receive();
        }

        public void receive() throws IOException {
            InputStream mmInputStream = mmSocket.getInputStream();
            byte[] buffer = new byte[256];
            int bytes;

            try {
                    bytes = mmInputStream.read(buffer);
                    String readMessage = new String(buffer, 0, bytes);

                    Log.d(TAG, "Received: " + readMessage);
                    TextView voltageLevel = (TextView) findViewById(R.id.Voltage);
                    voltageLevel.setText(readMessage);
                    mmSocket.close();
                } catch (IOException e) {
                    Log.e(TAG, "Problems occurred!");
                    return;
                }
            }
        }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_comms);

        BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        final Intent intent = getIntent();
        final String address = intent.getStringExtra(MainActivity.EXTRA_ADDRESS);
        Button vinButton = (Button) findViewById(R.id.getVin);
        Button dtcButton = (Button) findViewById(R.id.getDtc);
        Button newButton = (Button) findViewById(R.id.getNew);

        vinButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                final BluetoothDevice device = BTAdapter.getRemoteDevice(address);
                try {
                        new ConnectThread(device, "vin").start();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        dtcButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                final BluetoothDevice device = BTAdapter.getRemoteDevice(address);
                try {
                    new ConnectThread(device, "dtc").start();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        newButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                final BluetoothDevice device = BTAdapter.getRemoteDevice(address);
                try {
                    new ConnectThread(device, "new").start();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, 0);
        }
    }
    @Override
    protected void onStop() {
        super.onStop();
        try {
            mmSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}