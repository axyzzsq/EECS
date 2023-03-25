# HTLM  form

```html
<!-- 这是一个包含整个表单的div元素 -->
<div id="form-container" style="width: 500px; margin: 0 auto;">
  <!-- 这是一个包含表单标题的div元素 -->
  <div id="form-title" style="text-align: center; font-size: 24px;">
    <h1>注册表单</h1>
  </div>
  <!-- 这是一个包含表单内容的div元素 -->
  <div id="form-content" style="padding: 20px;">
    <!-- 这里使用了<form>元素来创建实际的表单 -->
    <form action="/register" method="post">
      <!-- 这里使用了<label>和<input>元素来创建输入字段 -->
      <label for="username">用户名：</label>
      <input type="text" id="username" name="username" required><br>
      <label for="password">密码：</label>
      <input type="password" id="password" name="password" required><br>
      <label for="email">邮箱：</label>
      <input type="email" id="email" name="email"><br>
      <!-- 这里使用了<input>元素来创建提交按钮 -->
      <input type="submit" value="注册">
    </form>
  </div>
</div>
```

