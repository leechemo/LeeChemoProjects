import { reactive } from 'vue';

const store = reactive({
  username: localStorage.getItem('username'),
  server_domain: "http://localhost:3001",
  mealPlan: [],
  preparationProgress: {},

  login(username) {
    localStorage.setItem('username', username);
    this.username = username;
    console.log("login", this.username);
  },

  logout() {
    // try {
    //   const res = await fetch(`${this.server_domain}/auth/Logout`, {
    //     method: 'POST',
    //     credentials: 'include'
    //   });
    //   const data = await res.json();
    //   console.log("logout response:", data.message);
    // } catch (e) {
    //   console.error("logout request failed", e);
    // }

    localStorage.removeItem('username');
    this.username = undefined;
    this.mealPlan = [];
    this.preparationProgress = {};
    console.log("logout completed – local state reset");
  },

  isLoggedIn() {
    return !!this.username;
  }

  // logout() {
  //   console.log("logout");
  //   localStorage.removeItem('username');
  //   this.username = undefined;
  // }


});

export default store;
