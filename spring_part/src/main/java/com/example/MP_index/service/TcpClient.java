package com.example.MP_index.service;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class TcpClient {
    private static final String END_POINT = "127.0.0.1";
    private static final int PORT =  54001;

    public static void sendUploadedImageToServer(String imageAbsPath){
        try {
            Socket socket = new Socket(END_POINT,PORT);
            //InputStream is = socket.getInputStream();
            OutputStream os = socket.getOutputStream();
            os.write(imageAbsPath.getBytes());

           // socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
