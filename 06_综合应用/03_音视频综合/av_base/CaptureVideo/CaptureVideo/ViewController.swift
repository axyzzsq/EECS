//
//  ViewController.swift
//  AudioResample
//
//  Created by lichao on 2020/2/9.
//  Copyright © 2020年 lichao. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {
    
    var recStatus: Bool = false;
    var thread: Thread?
    let btn = NSButton.init(title:"", target:nil, action:nil)

    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Do any additional setup after loading the view.
        self.view.setFrameSize(NSSize(width: 320, height: 240))
        
        btn.title = "开始录制"
        btn.bezelStyle = .rounded
        btn.setButtonType(.pushOnPushOff)
        btn.frame = NSRect(x:320/2-60,y: 240/2-15, width:120, height: 30)
        btn.target = self
        btn.action = #selector(myfunc)
        
        self.view.addSubview(btn)

        // Do any additional setup after loading the view.
    }
    
    @objc func myfunc(){
        //print("callback!")
        self.recStatus = !self.recStatus;
        
        if recStatus {
            thread = Thread.init(target: self,
                                 selector: #selector(self.recAudio),
                                 object: nil)
            thread?.start()
            
            self.btn.title = "停止录制"
        }else{
            set_status(0);
            self.btn.title = "开始录制"
        }
        //rec_audio()
    }
    
    @objc func recAudio(){
        //print("start thread")
        rec_video()
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }


}

