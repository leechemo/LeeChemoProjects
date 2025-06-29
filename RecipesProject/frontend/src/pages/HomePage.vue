<template>
  <div class="container-fluid py-4">
    <div class="row">
      <!-- Left Column - Random Recipes -->
      <div class="col-lg-6 mb-4">
        <div class="card shadow h-100 d-flex flex-column">
          <div class="card-header bg-primary text-white">
            <h4 class="mb-0">
              <i class="bi bi-compass"></i> Explore these recipes
            </h4>
          </div>
          <div class="card-body flex-grow-1">
            <div v-if="loadingRandom" class="text-center py-4">
              <div class="spinner-border text-primary" role="status">
                <span class="visually-hidden">Loading...</span>
              </div>
              <p class="mt-2 text-muted">Loading random recipes...</p>
            </div>
            <div v-else-if="randomRecipes.length > 0">
              <RecipePreviewList :title="''" :recipes="randomRecipes" />
            </div>
            <div v-else class="text-center py-4">
              <i class="bi bi-emoji-frown display-4 text-muted"></i>
              <p class="text-muted mt-2">No recipes found</p>
            </div>
          </div>
          <div class="card-footer bg-transparent border-0 text-center">
            <button 
              @click="loadRandomRecipes" 
              class="btn btn-outline-primary"
              :disabled="loadingRandom"
            >
              <span v-if="loadingRandom" class="spinner-border spinner-border-sm me-2" role="status"></span>
              <i v-else class="bi bi-arrow-clockwise me-2"></i>
              {{ loadingRandom ? 'Loading...' : 'More recipes' }}
            </button>
          </div>
        </div>
      </div>

      <!-- Right Column - User Specific Content -->
      <div class="col-lg-6 mb-4">
        <div class="card shadow h-100 d-flex flex-column">
          <div v-if="isLoggedIn" class="card-header bg-success text-white">
            <h4 class="mb-0">
              <i class="bi bi-clock-history"></i> Last watched recipes
            </h4>
          </div>
          <div v-else class="card-header bg-warning text-dark">
            <h4 class="mb-0">
              <i class="bi bi-person-circle"></i> Sign in to the site
            </h4>
          </div>
          <div class="card-body flex-grow-1">
            <!-- Logged in content -->
            <div v-if="isLoggedIn">
              <div v-if="loadingWatched" class="text-center py-4">
                <div class="spinner-border text-success" role="status">
                  <span class="visually-hidden">Loading...</span>
                </div>
                <p class="mt-2 text-muted">Loading recent recipes...</p>
              </div>
              <div v-else-if="watchedRecipes.length > 0">
                <RecipePreviewList :title="''" :recipes="watchedRecipes" :getRoute="getRecipeRoute" />
              </div>
              <div v-else class="text-center py-4">
                <i class="bi bi-eye-slash display-4 text-muted"></i>
                <p class="text-muted mt-2">You haven't viewed any recipes yet</p>
                <router-link to="/search" class="btn btn-outline-success">
                  <i class="bi bi-search me-2"></i>Search recipes
                </router-link>
              </div>
            </div>
            <!-- Non-logged in content -->
            <div v-else class="text-center py-5">
              <i class="bi bi-person-plus display-1 text-warning mb-4"></i>
              <h5 class="text-muted mb-3">Sign in to see personalized recipes</h5>
              <p class="text-muted mb-4">Get recipe recommendations tailored to your tastes and track the recipes you've viewed</p>
              <div class="d-grid gap-2 d-md-block">
                <router-link to="/login" class="btn btn-success btn-lg me-md-2">
                  <i class="bi bi-box-arrow-in-right me-2"></i>Sign In
                </router-link>
                <router-link to="/register" class="btn btn-outline-primary btn-lg">
                  <i class="bi bi-person-plus me-2"></i>Register
                </router-link>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios'
import RecipePreviewList from '@/components/RecipePreviewList.vue'
import store from '@/store'


export default {
  name: 'HomePage',
  components: { RecipePreviewList },
  data() {
    return {
      // isLoggedIn: false,
      loadingRandom: false,
      loadingWatched: false,
      randomRecipes: [],
      watchedRecipes: []
    }
  },
  computed: {
    isLoggedIn() {
      return store.isLoggedIn();
    }
  },
  mounted() {
    // this.checkLoginStatus()
    this.loadRandomRecipes();
    if (this.isLoggedIn) {
      this.loadWatchedRecipes();
    }
  },
  methods: {
    // checkLoginStatus() {
    //   this.isLoggedIn = !!localStorage.getItem('username') || !!this.$store?.state?.user
    //   if (this.isLoggedIn) {
    //     this.loadWatchedRecipes()
    //   }
    // },
    
    async loadRandomRecipes() {
      this.loadingRandom = true
      try {
        // קריאה אמיתית ל-backend
        const response = await axios.get('http://localhost:3001/recipes/random')
        console.log("Random recipes response:", response.data); // Log the response
        this.randomRecipes = response.data
      } catch (error) {
        console.error('Error loading random recipes:', error)
        this.toast('Error', 'Error loading random recipes', 'danger')
      } finally {
        this.loadingRandom = false
      }
    },
    
    async loadWatchedRecipes() {
      this.loadingWatched = true
      try {
        // קריאה אמיתית ל-backend
        const response = await axios.get('user/recipes/watched', {
          withCredentials: true,
          headers: {
            // אם יש צורך בטוקן, יש להוסיף אותו כאן
            // 'Authorization': `Bearer ${localStorage.getItem('token')}`
          }
        })
        this.watchedRecipes = response.data.map(recipe => ({
          ...recipe,
          isFamily: recipe.isFamily === true || recipe.isFamily === 1
        }));

      } catch (error) {
        console.error('Error loading watched recipes:', error)
        this.toast('Error', 'Error loading recent recipes', 'danger')
      } finally {
        this.loadingWatched = false
      }
    },
    
    viewRecipe(recipeId) {
      this.$router.push(`/recipe/${recipeId}`)
    },

    getRecipeRoute(recipe) {
      const recipeID = recipe.recipeID;
      const isFamily = recipe.isFamily === true || recipe.isFamily === 1;

      if (recipeID.startsWith('RU')) {
        if (isFamily) {
          return { name: 'FamilyRecipeView', params: { recipeID } };
        } else {
          return { name: 'MyRecipeView', params: { recipeID } };
        }
      } else {
        return { name: 'FullRecipeView', params: { recipeID } };
      }
  }
  },
  // watch: {
  //   isLoggedIn(newVal) {
  //     if (newVal) {
  //       this.loadWatchedRecipes()
  //     }
  //   }
  // }
}
</script>

<style scoped>
.recipe-item {
  cursor: pointer;
  transition: transform 0.2s, box-shadow 0.2s;
  border: 1px solid #dee2e6;
}

.recipe-item:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(0,0,0,0.15);
}

.recipe-meta {
  display: flex;
  flex-wrap: wrap;
  gap: 0.5rem;
}

.card-text {
  display: -webkit-box;
  -webkit-line-clamp: 2;
  line-clamp: 2;
  -webkit-box-orient: vertical;
  overflow: hidden;
  font-size: 0.9rem;
  color: #666;
}

.btn {
  border-radius: 8px;
  font-weight: 500;
}

.btn:hover {
  transform: translateY(-1px);
}

.card {
  border: none;
  border-radius: 15px;
}

.card-header {
  border-radius: 15px 15px 0 0 !important;
  border-bottom: none;
}
</style>
