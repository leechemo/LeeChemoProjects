import { createApp } from 'vue';
import App from './App.vue';
import router from './router';
import axios from 'axios';
import VueAxios from 'vue-axios';

import 'bootstrap/dist/css/bootstrap.css';
import 'bootstrap/dist/js/bootstrap.bundle.js';
import 'bootstrap-icons/font/bootstrap-icons.css';

import store from './store';

// Configure axios with backend URL
axios.defaults.baseURL = store.server_domain;

axios.defaults.withCredentials = true;


// // Create router with guards
// const router = createRouter({
//   history: createWebHistory(),
//   routes: routes.routes
// });

// // Apply route guards
// router.beforeEach(routes.beforeEach);
// router.afterEach(routes.afterEach);

const app = createApp(App);

app.use(router);
app.use(VueAxios, axios);

app.config.globalProperties.store = store;
app.config.globalProperties.axios = axios;

axios.get(`${store.server_domain}/session`, { withCredentials: true })
  .then((res) => {
    store.login(res.data.username);  
  })
  .catch(() => {
    store.logout(); 
  });


app.config.globalProperties.toast = function (title, content, variant = null, append = false) {
  const toastContainerId = "toast-container";
  let toastContainer = document.getElementById(toastContainerId);
  if (!toastContainer) {
    toastContainer = document.createElement("div");
    toastContainer.id = toastContainerId;
    toastContainer.style.position = "fixed";
    toastContainer.style.top = "1rem";
    toastContainer.style.right = "1rem";
    toastContainer.style.zIndex = "1055";
    document.body.appendChild(toastContainer);
  }

  const toast = document.createElement("div");
  toast.className = `toast align-items-center text-bg-${variant || 'info'} border-0 show`;
  toast.setAttribute("role", "alert");
  toast.setAttribute("aria-live", "assertive");
  toast.setAttribute("aria-atomic", "true");

  toast.innerHTML = `
    <div class="d-flex">
      <div class="toast-body">
        <strong>${title}</strong><br>${content}
      </div>
      <button type="button" class="btn-close btn-close-white me-2 m-auto" data-bs-dismiss="toast" aria-label="Close"></button>
    </div>
  `;

  if (!append) {
    toastContainer.innerHTML = "";
  }
  toastContainer.appendChild(toast);

  setTimeout(() => {
    toast.remove();
  }, 3000);
};

// Make toast globally available
window.toast = app.config.globalProperties.toast;

// Make axios globally available
window.axios = axios;
window.store = store; // @ts-ignore

const savedUsername = localStorage.getItem('username')|| localStorage.getItem('user');
if (savedUsername) {
  store.login(savedUsername); 
}

app.mount('#app');
