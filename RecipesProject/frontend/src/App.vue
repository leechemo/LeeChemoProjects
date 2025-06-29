<template>
  <div id="app" class="d-flex flex-column min-vh-100">
    <!-- Navigation Bar -->
    <nav class="navbar navbar-expand-lg navbar-dark bg-primary shadow-sm">
      <div class="container">
        <!-- Brand/Logo -->
        <router-link class="navbar-brand d-flex align-items-center" :to="{ name: 'home' }">
          <i class="bi bi-cup-hot me-2"></i>
          <span class="fw-bold">Recipes Website</span>
        </router-link>

        <!-- Mobile Toggle Button -->
        <button 
          class="navbar-toggler" 
          type="button" 
          data-bs-toggle="collapse" 
          data-bs-target="#navbarNav"
          aria-controls="navbarNav" 
          aria-expanded="false" 
          aria-label="Toggle navigation"
        >
          <span class="navbar-toggler-icon"></span>
        </button>

        <!-- Navigation Items -->
        <div class="collapse navbar-collapse" id="navbarNav">
          <ul class="navbar-nav me-auto">
            <li class="nav-item">
              <router-link class="nav-link" :to="{ name: 'home' }">
                <i class="bi bi-house-door me-1"></i>Home
              </router-link>
            </li>
            <li class="nav-item">
              <router-link class="nav-link" :to="{ name: 'search' }">
                <i class="bi bi-search me-1"></i>Search
              </router-link>
            </li>
            <li class="nav-item">
              <router-link class="nav-link" :to="{ name: 'about' }">
                <i class="bi bi-info-circle me-1"></i>About
              </router-link>
            </li>
          </ul>

          <!-- User Menu -->
          <ul class="navbar-nav">
            <!-- Guest User -->
            <template v-if="!isLoggedIn">
              <li class="nav-item">
                <router-link class="nav-link" :to="{ name: 'login' }">
                  <i class="bi bi-box-arrow-in-right me-1"></i>Sign In
                </router-link>
              </li>
              <li class="nav-item">
                <router-link class="nav-link btn btn-outline-light btn-sm ms-2" :to="{ name: 'register' }">
                  <i class="bi bi-person-plus me-1"></i>Register
                </router-link>
              </li>
            </template>

            <!-- Logged In User -->
            <template v-else>
              <li class="nav-item">
                <button @click="showCreateRecipeModal = true" class="nav-link btn btn-outline-light btn-sm me-2">
                  <i class="bi bi-plus-circle me-1"></i>Create Recipe
                </button>
              </li>


              <!-- User Dropdown -->
              <li class="nav-item dropdown">
                <a 
                  class="nav-link dropdown-toggle d-flex align-items-center" 
                  href="#" 
                  role="button" 
                  data-bs-toggle="dropdown" 
                  aria-expanded="false"
                >
                  <i class="bi bi-person-circle me-1"></i>
                  {{ username }}
                </a>
                <ul class="dropdown-menu dropdown-menu-end">
                  <li>
                    <router-link class="dropdown-item" :to="{ name: 'home' }">
                      <i class="bi bi-house-door me-2"></i>Home
                    </router-link>
                  </li>
                  <li>
                    <router-link class="dropdown-item" :to="{ name: 'search' }">
                      <i class="bi bi-search me-2"></i>Search
                    </router-link>
                  </li>
                  <li><hr class="dropdown-divider"></li>
                  <!-- <li>
                    <router-link class="dropdown-item" :to="{ name: 'profile' }">
                      <i class="bi bi-person me-2"></i>Profile
                    </router-link>
                  </li> -->
                  <li>
                    <router-link class="dropdown-item" :to="{ name: 'favorites' }">
                      <i class="bi bi-heart me-2"></i>Favorites
                    </router-link>
                  </li>
                  <li>
                    <router-link class="dropdown-item" :to="{ name: 'my-recipes' }">
                      <i class="bi bi-journal-text me-2"></i>My Recipes
                    </router-link>
                  </li>
                  <li>
                    <router-link class="dropdown-item" :to="{ name: 'family-recipes' }">
                      <i class="bi bi-people me-2"></i>Family Recipes
                    </router-link>
                  </li>
                  <li>
                    <router-link class="dropdown-item" :to="{ name: 'MealPlan' }">
                      <i class="bi bi-list-task me-2"></i>Meal Plan
                    </router-link>
                  </li>
                  <li><hr class="dropdown-divider"></li>
                  <li>
                    <a class="dropdown-item text-danger" href="#" @click.prevent="logout">
                      <i class="bi bi-box-arrow-right me-2"></i>Log Out
                    </a>
                  </li>
                </ul>
              </li>
            </template>
          </ul>
        </div>
      </div>
    </nav>

    <!-- Main Content -->
    <main class="main-content">
      <router-view />
    </main>

    <!-- Footer -->
    <footer class="bg-light text-center text-muted py-4 mt-5">
      <div class="container">
        <p class="mb-0">
          <i class="bi bi-heart-fill text-danger"></i>
           Recipes Website Of Adva, Shani and Lee
        </p>
      </div>
    </footer>

    <!-- Create Recipe Modal -->
    <div 
      v-if="showCreateRecipeModal" 
      class="modal fade show d-block" 
      tabindex="-1"
      style="background-color: rgba(0,0,0,0.5);"
    >
      <div class="modal-dialog modal-lg">
        <div class="modal-content">
          <div class="modal-header">
            <h5 class="modal-title">
              <i class="bi bi-plus-circle me-2"></i>Create New Recipe
            </h5>
            <button 
              @click="showCreateRecipeModal = false" 
              type="button" 
              class="btn-close"
            ></button>
          </div>
          <div class="modal-body">
            <div class="text-center py-5">
              <i class="bi bi-journal-text display-1 text-primary mb-3"></i>
              <h4>Create New Recipe</h4>
              <p class="text-muted mb-4">
                Go to "My Recipes" to create a new recipe with all the details
              </p>
              <router-link 
                :to="{ name: 'my-recipes' }" 
                class="btn btn-primary btn-lg"
                @click="showCreateRecipeModal = false"
              >
                <i class="bi bi-arrow-right me-2"></i>Go to "My Recipes"
              </router-link>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { getCurrentInstance, computed, onMounted, ref } from 'vue';

export default {
  name: "App",
  setup() {
    const internalInstance = getCurrentInstance();
    const store = internalInstance.appContext.config.globalProperties.store;
    const toast = internalInstance.appContext.config.globalProperties.toast;
    const router = internalInstance.appContext.config.globalProperties.$router;

    // Computed properties for reactive navigation
    const isLoggedIn = computed(() => {
      return !!store.username || !!localStorage.getItem('user');
      // return store.isLoggedIn();
    });

    const username = computed(() => {
      if (store.username) return store.username;
      const userStr = localStorage.getItem('user');
      if (userStr) {
        try {
          const userObj = JSON.parse(userStr);
          return userObj.username || 'User';
        } catch {
          return 'User';
        }
      }
      return 'User';
    });

    const showCreateRecipeModal = ref(false);

    const logout = async () => {
      try {
        const res = await fetch(`${store.server_domain}/auth/Logout`, {
          method: 'POST',
          credentials: 'include'
        });
        const data = await res.json();
        console.log("Logout server response:", data.message);
      } catch (err) {
        console.error("Failed to logout from server:", err);
      }
      // Clear store
      store.logout();
      
      // Clear localStorage
      localStorage.removeItem('user');
      localStorage.removeItem('rememberMe');
      localStorage.removeItem('username');
      
      // Show success message
      toast("You have successfully logged out of the system", "success");
      
      // Redirect to home
      router.push("/").catch(() => {});
    };

    // Check login status on mount
    onMounted(() => {
      // Initialize Bootstrap dropdowns
      if (typeof window !== 'undefined' && window.bootstrap) {
        const dropdownElementList = document.querySelectorAll('.dropdown-toggle');
        dropdownElementList.forEach(dropdownToggleEl => {
          new window.bootstrap.Dropdown(dropdownToggleEl);
        });
      }
    });

    return { 
      store, 
      logout, 
      isLoggedIn, 
      username, 
      showCreateRecipeModal 
    };
  }
}
</script>

<style lang="scss">
@import "@/scss/form-style.scss";

#app {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: #2c3e50;
  min-height: 100vh;
  display: flex;
  flex-direction: column;
}

.navbar {
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
}

.navbar-brand {
  font-size: 1.5rem;
  font-weight: 700;
}

.nav-link {
  font-weight: 500;
  transition: color 0.3s ease;
  
  &:hover {
    color: rgba(255, 255, 255, 0.8) !important;
  }
}

.nav-link.router-link-active {
  color: #fff !important;
  font-weight: 600;
}

.dropdown-item {
  transition: background-color 0.2s ease;
  
  &:hover {
    background-color: #f8f9fa;
  }
}

.main-content {
  flex: 1;
  min-height: calc(100vh - 200px); // Account for navbar and footer
}

footer {
  margin-top: auto;
  border-top: 1px solid #dee2e6;
}

// Responsive adjustments
@media (max-width: 768px) {
  .navbar-brand {
    font-size: 1.2rem;
  }
  
  .nav-link {
    padding: 0.5rem 0;
  }
}

// Custom scrollbar
::-webkit-scrollbar {
  width: 8px;
}

::-webkit-scrollbar-track {
  background: #f1f1f1;
}

::-webkit-scrollbar-thumb {
  background: #c1c1c1;
  border-radius: 4px;
}

::-webkit-scrollbar-thumb:hover {
  background: #a8a8a8;
}

.personal-dropdown-menu {
  border-left: 4px solid #17a2b8;
  border-radius: 0 0.5rem 0.5rem 0;
  padding-left: 0.5rem;
}
.personal-dropdown-menu .dropdown-item {
  display: flex;
  align-items: center;
  font-weight: 500;
  font-size: 1rem;
  padding-left: 1.5rem;
  border: none;
}
.personal-dropdown-menu .dropdown-item i {
  margin-left: 0.5rem;
  font-size: 1.1em;
}
</style>
