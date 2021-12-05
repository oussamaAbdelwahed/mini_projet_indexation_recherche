package com.example.MP_index.service;

import org.springframework.core.io.FileSystemResource;
import org.springframework.stereotype.Service;
import org.springframework.util.ResourceUtils;
import org.springframework.web.multipart.MultipartFile;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.UUID;

@Service
public class FileUploadService {
    private static final String PATH_SEPARATOR = "/";
    private static final String UPLOAD_DIR = new FileSystemResource("").getFile().getAbsolutePath()+PATH_SEPARATOR+"src"+PATH_SEPARATOR+"main"+PATH_SEPARATOR+"resources"+PATH_SEPARATOR+"static"+PATH_SEPARATOR+"request_images";
    
    private static final String ENDPOINT="127.0.0.1";
    private static final int PORT=8888;
    
    private static final String REQUEST_IMAGES_PATH="http://localhost:8080/request_images/";
    

    public String[] uploadFile(MultipartFile file,String ext,String descriptor) throws IllegalStateException, IOException{
    	String [] rawResults = null;
        try{
            UUID uuid = UUID.randomUUID();
            String randomUUIDString = uuid.toString();
            String requestImagePath = UPLOAD_DIR+PATH_SEPARATOR+randomUUIDString+"."+ext;
            Files.copy(file.getInputStream(), Paths.get(requestImagePath), StandardCopyOption.REPLACE_EXISTING);
            
            
            String tcpMessage = requestImagePath+";;"+descriptor;
            
            Socket socket = new Socket(ENDPOINT,PORT);
            
			OutputStream os = socket.getOutputStream();
			
			os.write(tcpMessage.getBytes());
			
			InputStream is = socket.getInputStream();
		    String res = "";
			int count;
			byte[] buffer = new byte[1024]; // or 4096, or more
			String str= null;
			
			while ((count = is.read(buffer)) > 0){
				str = new String(buffer);
				res+=str;
			}
			
			is.close();
			os.close();
			socket.close();
			
			rawResults = res.split(";");
        }catch (Exception e){
             e.printStackTrace();
        }
        return  rawResults;
    }
}

