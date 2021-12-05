package com.example.MP_index.controller;


import com.example.MP_index.service.FileUploadService;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.io.FileSystemResource;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Base64;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

@Controller
public class FileUploadController {
    @Autowired
    private FileUploadService srv;
    private static final String SERVER_BASE_URL="http://localhost:8080";
    
   

    @PostMapping(value = "/search")
    public String uploadFile(RedirectAttributes redirectAttributes, @RequestParam("descriptor") String descriptor,@RequestParam("img") MultipartFile img) throws IllegalStateException, IOException {
        String encodedBase64ReqImage = Base64.getEncoder().encodeToString(img.getBytes()); 
    	String [] res = srv.uploadFile(img,img.getContentType().split("/")[1],descriptor);
        String [] fullyQualifImageNames = new String[10];
        String [] mesuredDistances = new String[10];
        
        redirectAttributes.addFlashAttribute("requestImage",encodedBase64ReqImage);
        redirectAttributes.addFlashAttribute("descriptor",descriptor);
       
        
        for(int i=0;i<res.length -1;i++) {
        	if(res[i].indexOf("$") != -1) {
        		String [] tmp = res[i].split("\\$");
        		fullyQualifImageNames[i] = SERVER_BASE_URL+"/images/images_db/"+tmp[0];
        		mesuredDistances[i] = tmp[1];
        	}
        }
        redirectAttributes.addFlashAttribute("resultingImages", fullyQualifImageNames);
        redirectAttributes.addFlashAttribute("resultingScores", mesuredDistances);

        return "redirect:/result";
    }
    
    
    
    @GetMapping(value = "/result")
    public String showResults() {
    	return "result";
    }
 
}
