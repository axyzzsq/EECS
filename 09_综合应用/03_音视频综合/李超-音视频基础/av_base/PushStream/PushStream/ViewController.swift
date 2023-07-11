//
//  ViewController.swift
//  PushStream
//
//  Created by lichao on 2020/3/17.
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
        
        btn.title = "开始推流"
        btn.bezelStyle = .rounded
        btn.setButtonType(.pushOnPushOff)
        btn.frame = NSRect(x:320/2-60,y: 240/2-15, width:120, height: 30)
        btn.target = self
        btn.action = #selector(myfunc)
        
        self.view.addSubview(btn)
    }
    
    @objc func myfunc() {
        //print("callback!")
        self.recStatus = !self.recStatus;
        
        if recStatus {
            set_status(1);
            thread = Thread.init(target: self,
                                 selector: #selector(self.PublishStream),
                                 object: nil)
            thread?.start()
            
            self.btn.title = "停止推流"
        }else{
            set_status(0);
            self.btn.title = "开始推流"
        }
    }
    
    @objc func PublishStream(){
        //print("start thread")
        publish_stream();
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }


}

