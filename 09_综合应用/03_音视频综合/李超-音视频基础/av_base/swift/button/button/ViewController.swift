//
//  ViewController.swift
//  button
//
//  Created by lichao on 2020/1/29.
//  Copyright © 2020年 lichao. All rights reserved.
//

import Cocoa
import Foundation

class ViewController: NSViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        
        let w_Width = 320
        let w_Height = 240
        
        let o_x:Int = 320/2 - 40
        let o_y:Int = 240/2 - 15
        
        self.view.setFrameSize(NSSize(width: w_Width, height: w_Height))
        
        let cmdButton = NSButton.init()
        cmdButton.frame = NSRect(x: o_x, y: o_y, width: 80, height: 30)
        //cmdButton.bezelStyle = .rounded
        cmdButton.title = "录音"
        cmdButton.setButtonType(.pushOnPushOff);
        cmdButton.target = self
        cmdButton.action = #selector(self.myfunc) // 1. callC 之前要有self, 2. callC 后面不要加（）
        self.view.addSubview(cmdButton)
    }
    
    //增加响应函数
    @objc func myfunc() {
        print("hello world!")
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }

}

